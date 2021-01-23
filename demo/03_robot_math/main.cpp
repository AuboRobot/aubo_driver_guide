#include <iostream>
#include <aubo_driver/aubo_driver.h>

#define M_PI 3.14159265358979323846 /* pi */
using namespace arcs::aubo_driver;

int main(int argc, char *argv[])
{
    auto aubo_driver = createAuboDriver();

    // 根据IP和端口连接到控制器软件
    aubo_driver->connectToServer();

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

    } else {
        std::cout << "user login failed" << std::endl;
    }

    aubo_driver->logout();
    return 0;
}
