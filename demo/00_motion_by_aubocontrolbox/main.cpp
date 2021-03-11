#include <iostream>
#include <math.h>
#include <thread>
#include <aubo_driver/aubo_driver.h>

using namespace std;
using namespace arcs::aubo_driver;

double distance(const std::vector<double> &a, const std::vector<double> &b)
{
    double res = 0.;
    if (a.size() != b.size()) {
        return -1;
    }

    for (int i = 0; i < (int)a.size(); i++) {
        res += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(res);
}


int main()
{
    const double j_acc = 0.1;
    const double j_velc = 0.2;
    vector<double> current_joint;

    auto aubo_driver = createAuboDriver();  //不传参数ip默认为127.0.0.1,如果需要改变需传参
    aubo_driver->connectToServer();
    if (aubo_driver->login("user", "111", 50)) {
        cout << "login success" << endl;
        vector<string> name;
        name.push_back("actual_q");
        aubo_driver->setRtdeOutputRecipe(0, name, 200);
        aubo_driver->startRtdeTransmission(0);
        aubo_driver->syncRtdeTransmission(0, 50);

        vector<double> j_position1;
        j_position1.push_back(30.0 * 3.14 / 180);
        j_position1.push_back((-15) * 3.14 / 180);
        j_position1.push_back(100.0 * 3.14 / 180);
        j_position1.push_back(25.0 * 3.14 / 180);
        j_position1.push_back(90.0 * 3.14 / 180);
        j_position1.push_back(30.0 * 3.14 / 180);
        aubo_driver->moveJoint(j_position1, j_acc, j_acc);
        while (true) {
            current_joint = aubo_driver->getJointPositions();
            if ((current_joint.size() == aubo_driver->getRobotDof())) {
                double dis = distance(current_joint, j_position1);
                if (dis < 0.0001) {
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        cout << "机械臂到位1" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        vector<double> L_Position;
        L_Position.push_back(0.3);
        L_Position.push_back(0.3);
        L_Position.push_back(0.3);
        L_Position.push_back(3.14);
        L_Position.push_back(0);
        L_Position.push_back(1.57);
        const double L_acc = 0.1;
        const double L_Velc = 0.1;
        const double L_ARot = 0.1;
        aubo_driver->moveLine(L_Position, L_acc, L_Velc, L_ARot);
        std::vector<double> q(6, 0.); // 逆解起始点
        auto qout =
            aubo_driver->getInverseKinematics(L_Position, j_position1, 1e-6);
        while (true) {
            current_joint = aubo_driver->getJointPositions();
            if ((current_joint.size() == aubo_driver->getRobotDof())) {
                double dis = distance(current_joint, qout);
                if (dis < 0.0001) {
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        cout << "机械臂到位2" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        vector<double> j_position2;
        j_position2.push_back(0.0 * 3.14 / 180);
        j_position2.push_back((-15) * 3.14 / 180);
        j_position2.push_back(60.0 * 3.14 / 180);
        j_position2.push_back(25.0 * 3.14 / 180);
        j_position2.push_back(45.0 * 3.14 / 180);
        j_position2.push_back(0.0 * 3.14 / 180);
        aubo_driver->moveJoint(j_position2, j_acc, j_velc);
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            current_joint = aubo_driver->getJointPositions();
            if ((current_joint.size() == aubo_driver->getRobotDof())) {
                double dis = distance(current_joint, j_position2);
                if (dis < 0.0001) {
                    break;
                }
            }
        }
        cout << "机械臂到位3" << endl;
    }

    aubo_driver->waitForTerminate();

    return 0;
}
