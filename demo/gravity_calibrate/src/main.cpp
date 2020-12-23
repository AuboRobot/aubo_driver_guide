#include <getopt.h>
#include <iostream>
#include <math.h>
#include <thread>
#include "async_writer.h"
#include <unistd.h>
#include <string.h>
#include "Gravity.h"

#include "trajectory_io.h"

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
    printf("         %s [Options] [-- GravityCompensate_Options]\n"
           "Options:\n"
           "         -h|--help       this help\n"
           "         -v|--version    version of auboserver\n"
           "         -f|--file       trajectory file\n",
           pname);
}

// 运行离线轨迹
int main(int argc, char *argv[])
{
    int opt;
    const char *traj_file = nullptr;

    static struct option longopts[] = { { "version", 0, nullptr, 'v' },
                                        { "file", 0, nullptr, 'f' },
                                        { "help", 0, nullptr, 'h' },
                                        { 0, 0, 0, 0 } };

    while ((opt = getopt_long(argc, argv, "vhf:s:p:co:", longopts, nullptr)) !=
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
        }
    }

    TrajectoryIo input(traj_file);
    auto traj = input.parse();
    auto traj_sz = traj.size();
    if (traj_sz == 0) {
        // 没有轨迹
        std::cerr << "No waypoints" << std::endl;
        return 0;
    }

    std::vector<std::vector<double>> forces, poses;

    for (auto &&it : traj) {
        if (it.size() != 12) {
            std::cout << "Make sure there are 12 double nums a line"
                      << std::endl;
            exit(-1);
        }
        std::vector<double> tmp;
        tmp.assign(it.begin(), it.begin() + 6);
        poses.push_back(tmp);
        tmp.assign(it.begin() + 6, it.begin() + 12);
        forces.push_back(tmp);
    }

    std::cout << "Read file complete: " << forces.size() << " records"
              << std::endl;
    std::cout << "Calibrating..." << std::endl;

    auto gravity = new Gravity();
    auto res = gravity->Ide_vLeast(Grav(), forces, poses, forces.size());

    std::cout << "Gravity calibrate:\n";
    std::cout << "Force zero drift: " << res.Fx0 << ", " << res.Fy0 << ", "
              << res.Fz0 << ", " << res.Tx0 << ", " << res.Ty0 << ", "
              << res.Tz0 << "\n";
    std::cout << "mass: " << res.G / 9.81 << " (" << res.Lx << ", " << res.Ly
              << ", " << res.Lz << ")"
              << "\n";
    std::cout << "Base mount: "
              << "(" << res.l1 << ", " << res.l2 << ", " << res.l3 << ")"
              << std::endl;

    return 0;
}
