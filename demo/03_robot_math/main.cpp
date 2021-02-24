#include <iostream>
#include <aubo_driver/aubo_driver.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif

using namespace arcs::aubo_driver;

int main(int argc, char *argv[])
{
    auto addr = strdup(argv[0]);
    // auto addr = malloc(1000);
    free(addr);
    auto aubo_driver = createAuboDriver();

    // 根据IP和端口连接到控制器软件
    aubo_driver->connectToServer("127.0.0.1", 30001, 100);

    // 指定用户名密码登录
    if (aubo_driver->login("user", "111", 50)) {
        std::cout << "user login succeed" << std::endl;

        std::cout << std::endl
                  << "==================开始输出正逆解功能=============="
                  << std::endl;
        // 调用正解算法(已知关节角，求位姿)
        std::vector<double> pose;
        pose.push_back(0 * M_PI / 180);
        pose.push_back((-10.87) * M_PI / 180);
        pose.push_back((74.76) * M_PI / 180);
        pose.push_back((0.) * M_PI / 180);
        pose.push_back((89.75) * M_PI / 180);
        pose.push_back((0.) * M_PI / 180);
        auto xout = aubo_driver->getForwardKinematics(pose);
        std::cout << "getForwardKinematics: " << xout[0] << ", " << xout[1]
                  << ", " << xout[2] << ", " << xout[3] << ", " << xout[4]
                  << ", " << xout[5] << std::endl;

        // 调用逆解算法(已知位姿，求关节角)
        std::vector<double> qnear(6, 0.);
        auto qout = aubo_driver->getInverseKinematics(xout, qnear, 1e-6);
        std::cout << "getInverseKinematics: " << qout[0] * 180 / M_PI << ", "
                  << qout[1] * 180 / M_PI << ", " << qout[2] * 180 / M_PI
                  << ", " << qout[3] * 180 / M_PI << ", "
                  << qout[4] * 180 / M_PI << ", " << qout[5] * 180 / M_PI
                  << std::endl;

        std::cout << std::endl
                  << "==================调用四元数与欧拉角转换=============="
                  << std::endl;
        // 调用四元数与欧拉角转换
        std::vector<double> rpy;
        std::vector<double> quat;
        rpy.push_back(178.3858 / 180 * M_PI);   // RX
        rpy.push_back(2.304904 / 180 * M_PI);   // RY
        rpy.push_back(-178.86303 / 180 * M_PI); // RZ

        quat = aubo_driver->getRpyTransQuat(rpy);
        std::cout << "RpyTransQuat" << std::endl;
        for (auto it = quat.begin(); it != quat.end(); it++) {
            std::cout << *it << ", ";
        }
        std::cout << std::endl << "QuatTranRpy" << std::endl;
        rpy.clear();
        rpy = aubo_driver->getQuatTranRpy(quat);
        for (auto it = rpy.begin(); it != rpy.end(); it++) {
            std::cout << (*it) * 180 / M_PI << ", ";
        }

        aubo_driver->logout();
    } else {
        std::cout << "user login failed" << std::endl;
    }

    aubo_driver->logout();
    return 0;
}
