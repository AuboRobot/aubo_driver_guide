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

void printJointModeData(std::string name, std::vector<JointMode> jointmode)
{
    std::vector<JointMode> m_jointmode = jointmode;
    cout << name << endl;
    for (auto iter = m_jointmode.begin(); iter != m_jointmode.end(); iter++) {
        cout << (int)*iter << ", ";
    }
    cout << "" << endl;
}

void printVectorData(std::string name, std::vector<double> data)
{
    std::vector<double> m_data = data;
    cout << "" << endl;
    cout << name << endl;
    for (auto iter = m_data.begin(); iter != m_data.end(); iter++) {
        cout << *iter << ", ";
    }
    cout << "" << endl;
}

void getJointData(AuboDriver *m_instance)
{
    std::cout << " " << std::endl << "-----01getJointData" << std::endl;
    if (0 == m_instance->getJointPositions().size()) {
        m_instance->syncRtdeTransmission(0, 50);
    }

    printVectorData("JointPositions", m_instance->getJointPositions());
    printVectorData("JointVelocities", m_instance->getJointVelocities());
    printVectorData("JointCurrents", m_instance->getJointCurrents());
    printVectorData("JointTargetPositions",
                    m_instance->getJointTargetPositions());
    printVectorData("JointTargetVelocities",
                    m_instance->getJointTargetVelocities());
    printVectorData("JointTargetCurrents",
                    m_instance->getJointTargetCurrents());
    printVectorData("JointTemperatures", m_instance->getJointTemperatures());
    printVectorData("JointVoltages", m_instance->getJointVoltages());
    printJointModeData("JointModes", m_instance->getJointModes());
}

void getRunTimeMachine(AuboDriver *m_instance)
{
    std::cout << " " << std::endl << "-----02getRunTimeMachine" << std::endl;
}

void getTcpData(AuboDriver *m_instance)
{
    std::cout << " " << std::endl << "-----03getTcpData" << std::endl;
    printVectorData("TcpForce", m_instance->getTcpForce());
    printVectorData("TcpPose", m_instance->getTcpPose());
    printVectorData("TcpSpeed", m_instance->getTcpSpeed());
    //    m_instance->getTcpForce();
    //    m_instance->getTcpPose();
    //    m_instance->getTcpSpeed();
}

void getMasterBoard(AuboDriver *m_instance)
{
    std::cout << " " << std::endl << "-----04getmasterBoard" << std::endl;
    // 接口抛出异常 ValueExpired
    // m_instance->getRobotVoltage();
    // m_instance->getRobotCurrent();
}

void getToolData(AuboDriver *m_instance)
{
    std::cout << " " << std::endl << "-----05getToolData" << std::endl;
    // 大小为0
    // printVectorData("ToolAnalogInput", m_instance->getToolAnalogInput());
    std::cout << "ToolVoltage: " << m_instance->getToolVoltage() << std::endl;
    // 接口抛出异常 ValueExpired
    // m_instance->getToolCurrent();
    // m_instance->getIoCurrent();
}

void getRegisterData(AuboDriver *m_instance)
{
    std::cout << " " << std::endl << "-----06getRegisterData" << std::endl;
}

void setRtdeRealData(AuboDriver *m_instance)
{
    std::cout << " " << std::endl << "-----07setRtdeData " << std::endl;
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

        aubo_driver->setRobotPowerOn();
        aubo_driver->setRobotOperational();

        vector<string> name;
        name.push_back("target_q");
        name.push_back("target_qd");
        name.push_back("target_qdd");
        name.push_back("target_current");
        name.push_back("target_moment");
        name.push_back("actual_q");
        name.push_back("actual_qd");
        name.push_back("actual_TCP_force");
        name.push_back("actual_TCP_speed");
        name.push_back("actual_TCP_pose");
        name.push_back("actual_current");
        name.push_back("actual_joint_voltage");
        name.push_back("joint_control_output");
        name.push_back("joint_temperatures");
        name.push_back("joint_mode");
        aubo_driver->setRtdeOutputRecipe(0, name, 200);
        aubo_driver->startRtdeTransmission(0, bind(getJointData, aubo_driver));
        aubo_driver->syncRtdeTransmission(0, 50);

        name.clear();
        name.push_back("actual_execution_time");
        name.push_back("robot_mode");
        name.push_back("safety_mode");
        name.push_back("safety_status");
        name.push_back("runtime_state");
        name.push_back("robot_status_bits");
        name.push_back("safety_status_bits");
        name.push_back("speed_scaling");
        name.push_back("target_speed_fraction");
        aubo_driver->setRtdeOutputRecipe(1, name, 200);
        aubo_driver->startRtdeTransmission(
            1, bind(getRunTimeMachine, aubo_driver));
        aubo_driver->syncRtdeTransmission(1, 50);

        name.clear();
        name.push_back("actual_TCP_pose");
        name.push_back("actual_TCP_speed");
        name.push_back("actual_TCP_force");
        name.push_back("target_TCP_pose");
        name.push_back("target_TCP_speed");
        name.push_back("elbow_position");
        name.push_back("elbow_velocity");
        name.push_back("actual_momentum");
        name.push_back("tcp_force_scalar");
        aubo_driver->setRtdeOutputRecipe(2, name, 200);
        aubo_driver->startRtdeTransmission(2, bind(getTcpData, aubo_driver));
        aubo_driver->syncRtdeTransmission(2, 50);

        name.clear();
        name.push_back("actual_digital_input_bits");
        name.push_back("actual_main_voltage");
        name.push_back("actual_robot_voltage");
        name.push_back("actual_robot_current");
        name.push_back("actual_digital_output_bits");
        name.push_back("operationalModeSelectorInput");
        name.push_back("threePositionEnablingDeviceInput");
        name.push_back("masterboard_temperature");
        name.push_back("analog_io_types");
        name.push_back("standard_analog_input0");
        name.push_back("standard_analog_input1");
        name.push_back("standard_analog_output0");
        name.push_back("standard_analog_output1");
        name.push_back("master_io_current");
        name.push_back("euromap67_input_bits");
        name.push_back("euromap67_output_bits");
        name.push_back("euromap67_24V_voltage");
        name.push_back("euromap67_24V_current");
        aubo_driver->setRtdeOutputRecipe(3, name, 200);
        aubo_driver->startRtdeTransmission(3,
                                           bind(getMasterBoard, aubo_driver));
        aubo_driver->syncRtdeTransmission(3, 50);

        name.clear();
        name.push_back("tool_mode");
        name.push_back("tool_output_mode");
        name.push_back("tool_digital_output0_mode");
        name.push_back("tool_digital_output1_mode");
        name.push_back("tool_analog_input_types");
        name.push_back("tool_analog_input0");
        name.push_back("tool_analog_input1");
        name.push_back("tool_output_voltage");
        name.push_back("tool_output_current");
        name.push_back("tool_voltage_48V");
        name.push_back("tool_current");
        name.push_back("tool_temperature");
        name.push_back("actual_tool_accelerometer");
        aubo_driver->setRtdeOutputRecipe(4, name, 200);
        aubo_driver->startRtdeTransmission(4, bind(getToolData, aubo_driver));
        aubo_driver->syncRtdeTransmission(4, 50);

        name.clear();
        name.push_back("output_bit_registers_0_to_63");
        name.push_back("output_bit_registers_64_to_127");
        name.push_back("output_int_registers");
        name.push_back("output_double_registers");
        name.push_back("input_bit_registers_0_to_63");
        name.push_back("input_bit_registers_64_to_127");
        name.push_back("input_int_registers");
        name.push_back("input_double_registers");
        aubo_driver->setRtdeOutputRecipe(5, name, 200);
        aubo_driver->startRtdeTransmission(5,
                                           bind(getRegisterData, aubo_driver));
        aubo_driver->syncRtdeTransmission(5, 50);

        name.clear();
        // name.push_back("speed_slider_mask");
        // name.push_back("speed_slider_fraction");
        // name.push_back("standard_digital_output_mask");
        // name.push_back("standard_digital_output");
        // name.push_back("configurable_digital_output_mask");
        // name.push_back("configurable_digital_output");
        // name.push_back("tool_digital_output_mask");
        // name.push_back("tool_digital_output");
        // name.push_back("standard_analog_output_mask");
        // name.push_back("standard_analog_output_type");
        // name.push_back("standard_analog_output_0");
        // name.push_back("standard_analog_output_1");
        // name.push_back("debug");
        // name.push_back("input_bit_registers0_to_31");
        // name.push_back("input_bit_registers32_to_63");
        // name.push_back("input_bit_registers64_to_127");
        name.push_back("input_int_registers");
        name.push_back("input_double_registers");
        aubo_driver->setRtdeOutputRecipe(6, name, 200);
        aubo_driver->startRtdeTransmission(6,
                                           bind(setRtdeRealData, aubo_driver));
        aubo_driver->syncRtdeTransmission(6, 50);

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
