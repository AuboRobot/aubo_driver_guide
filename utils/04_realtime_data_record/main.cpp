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

std::vector<double> operator+(const std::vector<double> &a,
                              const std::vector<double> &b)
{
    std::vector<double> res(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        res[i] = a[i] + b[i];
    }
    return res;
}

std::vector<double> operator-(const std::vector<double> &a,
                              const std::vector<double> &b)
{
    std::vector<double> res(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        res[i] = a[i] - b[i];
    }
    return res;
}

std::vector<double> operator/(const std::vector<double> &a, const double &b)
{
    std::vector<double> res(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        res[i] = a[i] / b;
    }
    return res;
}

template <size_t N>
class GlideFileter
{
public:
    explicit GlideFileter(const int &dof) : dof_(dof)
    {
        for (size_t i = 0; i < N; i++) {
            records_[i].assign(dof_, 0.);
        }
    }

    std::vector<double> eval(const std::vector<double> &sample)
    {
        records_[index_] = sample;
        if (index_ == (N - 1)) {
            index_ = 0;
        } else {
            index_++;
        }
        if (size_ < N) {
            size_++;
        }

        std::vector<double> total(dof_, 0);

        for (size_t i = 0; i < N; i++) {
            total = total + records_[i];
        }

        if (size_ < N) {
            return total / size_;
        }
        return total / N;
    }

private:
    std::array<std::vector<double>, N> records_;
    size_t index_ = 0;
    size_t size_ = 0;
    int dof_ = -1;
};

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
           "         -d|--duration     sample duration\n"
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
    int duration = 5;
    bool app = false;
    const char *out_file = "track.csv";

    static struct option longopts[] = {
        { "version", 0, nullptr, 'v' },  { "help", 0, nullptr, 'h' },
        { "host", 0, nullptr, 's' },     { "port", 0, nullptr, 'p' },
        { "duration", 0, nullptr, 'd' }, { "output", 0, nullptr, 'o' },
        { "append", 0, nullptr, 'a' },   { 0, 0, 0, 0 }
    };

    while ((opt = getopt_long(argc, argv, "vhs:p:d:o:a", longopts, nullptr)) !=
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
        case 's':
            ip = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 'd':
            duration = atoi(optarg);
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

    AsyncBuf buffer(out_file, app);
    std::ostream astream(&buffer);
    // 连接到控制器软件
    aubo_driver->connectToServer(
        ip, port); //不传参数ip默认为127.0.0.1,如果需要改变需传参

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

        GlideFileter<10> filter(6);
        aubo_driver->startRtdeTransmission(0, [aubo_driver, &astream, &filter] {
            // astream << aubo_driver->getJointPositions() << ", ";
            // astream << aubo_driver->getJointTargetPositions() << ", ";
            // astream << aubo_driver->getJointVelocities() << ", ";
            // astream << aubo_driver->getJointCurrents() << ", ";
            // astream << aubo_driver->getJointTemperatures() << ", ";
            astream << filter.eval(aubo_driver->getTcpForce()) << ", ";
            astream << aubo_driver->getTcpForce();
            astream << "\n";
            // std::cout << aubo_driver->getTcpForce() << std::endl;
        });
        std::this_thread::sleep_for(std::chrono::seconds(duration));
        std::cout << "Record done, now flush to file" << std::endl;

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
