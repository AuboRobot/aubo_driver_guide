#include <iostream>
#include <thread>
#include <math.h>
#include <future>
#include <aubo_driver/aubo_driver.h>
#include <trajectory_io.h>

using namespace std;
using namespace arcs::aubo_driver;
promise<bool> G_pro;
future<bool> G_fut = G_pro.get_future();

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

void getRtdeData(AuboDriver *m_instance)
{
    if (0 == m_instance->getJointPositions().size()) {
        m_instance->syncRtdeTransmission(0, 50);
    }
    std::vector<double> m_joint = m_instance->getJointPositions();
    for (auto iter = m_joint.begin(); iter != m_joint.end(); iter++) {
        cout << *iter << endl;
    }
    cout << "" << endl;
    m_instance->getJointVelocities();
    m_instance->getTcpForce();
    m_instance->getTcpPose();
    //    m_instance->getTcpSpeed();
    m_instance->getJointCurrents();
    m_instance->getJointVelocities();
    //    m_instance->getIoCurrent();
    //    m_instance->getToolDigitalInput();
    //    m_instance->getToolDigitalOutput();
    //    m_instance->getConfigurableDigitalInput();
    //    m_instance->getConfigurableDigitalOutput();
    //    m_instance->getStandardDigitalInput();
    //    m_instance->getStandardDigitalOutput();
}

void setRtdeData(AuboDriver *m_instance, std::vector<std::vector<double>> traj)
{
    bool startrtde = false;
    while (true) {
        if (m_instance != nullptr) {
            if (!startrtde) {
                if (G_fut.get()) {
                    startrtde = true;
                }
            }

            if (startrtde) {
                for (auto it : traj) {
                    if ((int)it.size() == m_instance->getRobotDof()) {
                        // 发送轨迹点到机器人
                        auto builder = m_instance->getRtdeInputBuilder();
                        builder->servoJoint(it);
                        builder->send();
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
                break;
            }
        }
    }
}

int main()
{
    TrajectoryIo input("record3.offt");
    auto traj = input.parse();

    auto aubo_driver = createAuboDriver();
    std::thread set_rtde_thread(setRtdeData, aubo_driver, traj);

    map<string, string> out_list = aubo_driver->getRtdeOutputList();
    map<string, string> in_list = aubo_driver->getRtdeInputList();
    for (auto it : out_list) {
        cout << "输出配方：" << it.first << ":" << it.second << endl;
    }

    for (auto it : in_list) {
        cout << "输入配方：" << it.first << ":" << it.second << endl;
    }

    aubo_driver->connectToServer();

    if (aubo_driver->login("user", "111", 50)) {
        cout << "login success" << endl;
        vector<string> name;
        name.push_back("actual_q");
        name.push_back("actual_qd");
        name.push_back("actual_TCP_force");
        name.push_back("actual_TCP_speed");
        name.push_back("actual_TCP_pose");
        name.push_back("actual_current");
        name.push_back("actual_joint_voltage");
        name.push_back("actual_digital_input_bits");
        name.push_back("actual_digital_output_bits");
        //        name.push_back("output_bit_registers_0_to_63");
        //        name.push_back("output_bit_registers_64_to_127");
        //        name.push_back("output_int_registers");
        //        name.push_back("output_double_registers");
        //        name.push_back("input_bit_registers_0_to_63");
        //        name.push_back("input_bit_registers_64_to_127");
        //        name.push_back("input_int_registers");
        //        name.push_back("input_double_registers");

        aubo_driver->setRtdeOutputRecipe(0, name, 200);
        aubo_driver->startRtdeTransmission(0, bind(getRtdeData, aubo_driver));
        aubo_driver->syncRtdeTransmission(0, 50);

        auto first_point = traj[0];
        aubo_driver->moveJoint(first_point, 1, 90 * M_PI / 180.);

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            double dis =
                distance(aubo_driver->getJointPositions(), first_point);
            if (dis < 0.1) {
                break;
            }
            std::cout << "distance: " << dis << std::endl;
        }
        G_pro.set_value(true);
    }

    aubo_driver->waitForTerminate();

    return 0;
}
