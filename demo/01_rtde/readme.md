#### 注意：运行前需要将 record 目录下的 record3.offt 文件拷贝到可执行程序所在目录



#### rtde调用步骤说明：



**1.创建 aubo_driver 实例**

```c++
auto aubo_driver = createAuboDriver();
```



**2.获取 rtde 的输入输出配方**

```c++
map<string, string> out_list = aubo_driver->getRtdeOutputList();
map<string, string> in_list = aubo_driver->getRtdeInputList();

for (auto it : out_list) {
    cout << "输出配方：" << it.first << ":" << it.second << endl;
}
for (auto it : in_list) {
    cout << "输入配方：" << it.first << ":" << it.second << endl;
}
```



**3.连接到服务器**

```c++
aubo_driver->connectToServer();
```



**4.登录**

```c++
aubo_driver->login("user", "111", 50)
```



**5.设置 rtde 输出菜单**

```c++
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
//name.push_back("output_bit_registers_0_to_63");
//name.push_back("output_bit_registers_64_to_127");
//name.push_back("output_int_registers");
//name.push_back("output_double_registers");
//name.push_back("input_bit_registers_0_to_63");
//name.push_back("input_bit_registers_64_to_127");
//name.push_back("input_int_registers");
//name.push_back("input_double_registers");

aubo_driver->setRtdeOutputRecipe(0, name, 200);
```



**6.开始指定编号的rtde输出：注册回调函数**

```c++
//回调函数
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
    //m_instance->getTcpSpeed();
    m_instance->getJointCurrents();
    m_instance->getJointVelocities();
    //m_instance->getIoCurrent();
    //m_instance->getToolDigitalInput();
    //m_instance->getToolDigitalOutput();
    //m_instance->getConfigurableDigitalInput();
    //m_instance->getConfigurableDigitalOutput();
    //m_instance->getStandardDigitalInput();
    //m_instance->getStandardDigitalOutput();
}

//开始回调
aubo_driver->startRtdeTransmission(0, bind(getRtdeData, aubo_driver));
```



**7.同步最新一次RTDE输出( 等待接收到输出数据包 )**

```c++
aubo_driver->syncRtdeTransmission(0, 50);
```



**8.等待与服务器断开连接（阻塞)**

```c++
aubo_driver->waitForTerminate();
```



**9.程序结束**