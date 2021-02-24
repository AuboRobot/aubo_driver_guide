#include <getopt.h>
#include <iostream>
#include <math.h>
#include <thread>
#include <async_writer.h>
#include <aubo_driver/aubo_driver.h>
#include <unistd.h>
#include <string.h>

#include "trajectory_io.h"

using namespace arcs::aubo_driver;
using namespace arcs::cstone;

double distance(const std::vector<double> &a, const std::vector<double> &b)
{
    double res = 0;
    if (a.size() != b.size()) {
        std::cerr << a.size() << b.size() << std::endl;
        return std::numeric_limits<double>::max();
    }
    for (int i = 0; i < (int)a.size(); i++) {
        res += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(res);
}

std::ostream &operator<<(std::ostream &out, const std::vector<double> &a)
{
    // using std::operator<<;
    int i = 0;
    for (auto &&it : a) {
        out << it;
        if (i++ != (int)(a.size() - 1)) {
            out << ", ";
        }
    }
    return out;
}

void printVersion()
{
    char versionInfo[64] = { 0 };
    // sprintf(versionInfo, "V%s.%s%s", 0, (0), "-Beta");
    printf("version: %s\n", versionInfo);
    printf("Built: %s %s\n", __DATE__, __TIME__);
}

void printUsage(char *pname)
{
    printf("Usage: \n");
    printf("         %s [Options] [-- OfflineTrajectory_Options]\n"
           "Options:\n"
           "         -h|--help       this help\n"
           "         -v|--version    version of auboserver\n"
           "         -f|--file       trajectory file\n"
           "         -s|--host       aubo control ip address\n"
           "         -p|--port       server's port\n"
           "         -c|--servoc     indicate trajectory is servoc\n"
           "         -o|--output     specify output file\n"
           "         -a|--append     output append to the file\n",
           pname);
}

// 运行离线轨迹
int main(int argc, char *argv[])
{
    int opt;
    std::string ip = "127.0.0.1";
    int port = 30001;
    bool is_servoj = true;
    bool app = false;
    const char *traj_file = nullptr;
    const char *out_file = "track.csv";

    static struct option longopts[] = { { "version", 0, nullptr, 'v' },
                                        { "file", 0, nullptr, 'f' },
                                        { "help", 0, nullptr, 'h' },
                                        { "host", 0, nullptr, 's' },
                                        { "port", 0, nullptr, 'p' },
                                        { "servoc", 0, nullptr, 'c' },
                                        { "output", 0, nullptr, 'o' },
                                        { "append", 0, nullptr, 'a' },
                                        { 0, 0, 0, 0 } };

    while ((opt = getopt_long(argc, argv, "vhf:s:p:co:a", longopts, nullptr)) !=
           -1) {
        switch (opt) {
        case 'h':
            printUsage(argv[0]);
            exit(0);
            break;
        case 'v':
            printVersion();
            exit(0);
            break;
        case 'f':
            traj_file = strdup(optarg);
            break;
        case 's':
            ip = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 'c':
            is_servoj = false;
            break;
        case 'o':
            out_file = strdup(optarg);
            break;
        case 'a':
            app = true;
            break;
        }
    }

    auto aubo_driver = createAuboDriver();
    TrajectoryIo input(traj_file);
    auto traj = input.parse();
    auto traj_sz = traj.size();
    if (traj_sz == 0) {
        // 没有轨迹
        std::cerr << "No waypoints" << std::endl;
        return 0;
    }

    AsyncBuf buffer(out_file, app);
    std::ostream astream(&buffer);
    // 连接到控制器软件
    aubo_driver->connectToServer(ip, port);

    if (aubo_driver->login("offline_trajectory", "111", 50)) {
        std::cout << "user login succeed" << std::endl;
        // aubo_driver->setRobotPowerOn();

        // 配置RTDE
        std::vector<std::string> names;
        names.push_back("actual_q");
        names.push_back("target_q");
        names.push_back("actual_qd");
        names.push_back("actual_current");
        names.push_back("actual_TCP_force");
        names.push_back("actual_TCP_pose");
        names.push_back("joint_temperatures");
        aubo_driver->setRtdeOutputRecipe(0, names, 200);

        aubo_driver->startRtdeTransmission(0);
        // usleep(5000);
        aubo_driver->syncRtdeTransmission(0, 1000);

        std::vector<double> q_init;
        if (is_servoj) {
            q_init = traj[0];
        } else {
            auto qnear = aubo_driver->getJointPositions();
            q_init = aubo_driver->getInverseKinematics(traj[0], qnear);
        }

        aubo_driver->moveJoint(q_init, 1, 90 * M_PI / 180.);
        while (true) {
            double dis = distance(aubo_driver->getJointPositions(), q_init);
            if (dis < 0.0001) {
                break;
            }
            std::cout << "distance: " << dis << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        std::cout << "Arrived at the first target waypoint: "
                  << aubo_driver->getJointPositions() << std::endl;

        aubo_driver->startRtdeTransmission(0, [aubo_driver, &astream] {
            // astream << aubo_driver->getJointPositions() << ", ";
            // astream << aubo_driver->getJointTargetPositions() << ", ";
            // astream << aubo_driver->getJointVelocities() << ", ";
            // astream << aubo_driver->getJointCurrents() << ", ";
            // astream << aubo_driver->getJointTemperatures() << ", ";
            astream << aubo_driver->getTcpPose() << ", ";
            astream << aubo_driver->getTcpForce();
            astream << "\n";
        });

        int i = 1;
        for (auto it : traj) {
            std::cout << "[" << i++ << "/" << traj_sz << "]"
                      << " _q_ " << it << std::endl;
            if ((int)it.size() == aubo_driver->getRobotDof()) {
                // 发送轨迹点到机器人
                auto builder = aubo_driver->getRtdeInputBuilder();
                if (is_servoj) {
                    builder->servoJoint(it);
                } else {
                    builder->servoCartesian(it);
                }
                builder->send();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        aubo_driver->pauseRtdeTransmission(0);
        astream << std::flush;
        std::cout << "Realtime data has been writen to " << out_file
                  << "\n"
                  // << "JointPositions(6 colums), "
                  // << "JointTargetPositions(6 colums), "
                  // << "JointVelocities(6 colums), "
                  // << "JointCurrents(6 colums), "
                  // << "JointTemperatures(6 colums), "
                  << "TcpPose(6 colums), "
                  << "TcpForce(6 colums)" << std::endl;
    } else {
        std::cout << "user login failed" << std::endl;
    }

    aubo_driver->disconnectFromServer();
    return 0;
}
