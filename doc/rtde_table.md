## rtde接口表(aubo_driver_0.4.0)



### 输入

| 序号 | 配方名称                         | 含义                 | 接口                                                         | 开放情况 |
| :--: | :------------------------------- | :------------------- | :----------------------------------------------------------- | :------: |
|  1   | speed_slider_mask                |                      |                                                              |    否    |
|  2   | speed_slider_fraction            | 速度比例             | void setSpeedFractionSlider(const double &f)                 |    是    |
|  3   | standard_digital_output_mask     | 标准数字输出位       | void setStandardDigitalOutput(const uint32_t &mask,const uint32_t &bits) |    是    |
|  4   | standard_digital_output          | 标准数字输出值       | 同上                                                         |    是    |
|  5   | configurable_digital_output_mask | 配置数字输出位       | void setConfigurableDigitalOutput(const uint32_t &mask,const uint32_t &bits) |    是    |
|  6   | configurable_digital_output      | 配置数字输出值       | 同上                                                         |    是    |
|  7   | tool_digital_output_mask         | 工具端输出位         | void setToolDigitalOutput(const uint32_t &mask,const uint32_t &bits) |    是    |
|  8   | tool_digital_output              | 工具端输出值         | 同上                                                         |    是    |
|  9   | standard_analog_output_mask      | 标准模拟量输出位     | void setStandardAnalogOutput(const int &index,const double &value); |    是    |
|  10  | standard_analog_output_type      | 标准模拟量输出类型   | 同上                                                         |    是    |
|  11  | standard_analog_output_0         | 标准模拟量输出0的值  |                                                              |    否    |
|  12  | standard_analog_output_1         | 标准模拟量输出1的值  |                                                              |    否    |
|  13  | debug                            | 调试信息             |                                                              |    否    |
|  14  | input_bit_registers0_to_31       | 寄存器输入位0~31     | void setBitRegisters0To31(const uint32_t &value);            |    是    |
|  15  | input_bit_registers32_to_63      | 寄存器输入位32~63    | void setBitRegisters32To63(const uint32_t &value);           |    是    |
|  16  | input_bit_registers64_to_127     | 寄存器输入位64~127   | void setBitRegisters64To127(const uint64_t &value);          |    是    |
|  17  | input_int_registers              | 寄存器输入int类型    | void setIntRegisters(const std::vector<int> &value)          |    是    |
|  18  | input_double_registers           | 寄存器输入double类型 | void setDoubleRegisters(const std::vector<double> &value)    |    是    |



### 输出

| 序号 | 配方名称  | 含义   | 接口 | 开放情况 |
| ---- | --------- | ------ | ---- | :------: |
| 1    | recipe_id | 配方id |      |    否    |
| 2    | timestamp | 时间戳 |      |    否    |

#### 关节

| 序号 | 配方名称             | 含义                   | 接口                                               | 开放情况 |
| ---- | -------------------- | ---------------------- | -------------------------------------------------- | :------: |
| 1    | target_q             | 目标关节角             | std::vector<double> getJointTargetPositions();     |    是    |
| 2    | target_qd            | 目标关节速度           | std::vector<double> getJointTargetVelocities();    |    是    |
| 3    | target_qdd           | 目标关节加速度         | std::vector<double> getJointTargetAccelerations(); |    是    |
| 4    | target_current       | 目标电流               | std::vector<double> getJointTargetCurrents();      |    是    |
| 5    | target_moment        | 目标关节力矩           | std::vector<double> getJointTargetTorques();       |    是    |
| 6    | actual_q             | 当前关节角             | std::vector<double> getJointPositions();           |    是    |
| 7    | actual_qd            | 当前关节速度           | std::vector<double> getJointVelocities();          |    是    |
| 8    | actual_current       | 当前关节电流           | std::vector<double> getJointCurrents();            |    是    |
| 9    |                      | 关节力矩               | std::vector<double> getJointTorques();             |    否    |
| 10   | joint_control_output | **关节电流输出(重复)** |                                                    |    否    |
| 11   | joint_temperatures   | 关节温度               | std::vector<double> getJointTemperatures();        |    是    |
| 12   | actual_joint_voltage | 当前关节电压           | std::vector<double> getJointVoltages();            |    是    |
| 13   | joint_mode           | 当前关节状态           | std::vector<JointMode> getJointModes();            |    是    |

#### 运行时主机(RunTimemachine)

| 序号 | 配方名称              | 含义                                                         | 接口                                    | 开放情况 |
| ---- | --------------------- | ------------------------------------------------------------ | --------------------------------------- | :------: |
| 1    | actual_execution_time | 实时线程运行时间                                             | double getActualExecutionTime();        |    是    |
| 2    | robot_mode            | 机器人模式                                                   | RobotMode getRobotMode();               |    是    |
| 3    | safety_mode           | 安全模式                                                     | SafetyMode getSafetyMode();             |    是    |
| 4    | safety_status         | 安全状态<br/>Normal = 1,<br/>Reduced = 2,<br/>ProtectiveStop = 3,<br/>Recovery = 4,<br/>SafeguardStop = 5,<br/>SystemEmergencyStop = 6,<br/>RobotEmergencyStop = 7,<br/>Violation = 8,<br/>Fault = 9,<br/>ValidateJointId = 10,<br/>Undefined = 11,<br/>AutomaticSafeguardStop = 12,<br/>ThreePositionEnablingStop = 13 | SafetyModeStatus getSafetyModeStatus(); |    是    |
| 5    | runtime_state         | 运行状态                                                     | RuntimeState getProgramRunState();      |    是    |
| 6    | robot_status_bits     | Bits 0-3:<br/>                 Is power on    <br/>                 Is program running         <br/>                 Is teach button pressed  <br/>                 Is power button pressed | uint32_t getRobotStatusBits();          |    是    |
| 7    | safety_status_bits    | Bits 0-10:   <br/>                 Is normal mode     <br/>                 Is reduced mode         <br/>                 Is protective stopped   <br/>                 Is recovery mode        <br/>                 Is safeguard stopped    <br/>                 Is system emergency stopped <br/>                 Is robot emergency stopped  <br/>                 Is emergency stopped      <br/>                 Is violation             <br/>                 Is fault                 <br/>                 Is stopped due to safety | uint32_t getSafetyStatusBits();         |    是    |
| 8    | speed_scaling         | 轨迹运行时候速度比例因子                                     | double getSpeedScaling();               |    是    |
| 9    | target_speed_fraction | 目标速度比例因子                                             | double getTargetSpeedFraction();        |    是    |

#### 工具中心位置(Tcp)

| 序号 | 配方名称         | 含义                             | 接口                                     | 开放情况 |
| ---- | ---------------- | -------------------------------- | ---------------------------------------- | :------: |
| 1    | actual_TCP_pose  | 当前工具的笛卡尔坐标             | std::vector<double> getTcpPose();        |    是    |
| 2    | actual_TCP_speed | 当前工具在笛卡尔坐标下的速度     | std::vector<double> getTcpSpeed();       |    是    |
| 3    | actual_TCP_force | 当前工具在笛卡尔坐标下的力矩     | std::vector<double> getTcpForce();       |    是    |
| 4    | target_TCP_pose  | 目标工具在笛卡尔空间下的坐标     | std::vector<double> getTcpTargetPose();  |    是    |
| 5    | target_TCP_speed | 目标工具在笛卡尔空间坐标下的速度 | std::vector<double> getTcpTargetSpeed(); |    是    |
| 6    | elbow_position   | 机器人肘部（关节3）位置          |                                          |    否    |
| 7    | elbow_velocity   | 机器人肘部（关节3）速度          |                                          |    否    |
| 8    | actual_momentum  | 当前动量                         |                                          |    否    |
| 9    | tcp_force_scalar | 工具端力矩标量                   |                                          |    否    |

#### 主接口板(MasterBoard)

| 序号 | 配方名称                         | 含义                                                         | 接口                                               | 开放情况 |
| ---- | -------------------------------- | ------------------------------------------------------------ | -------------------------------------------------- | :------: |
| 1    | actual_digital_input_bits        | 当前数字输入的状态<br>0-7:标准IO，8-15:配置IO<br>**16-17工具IO(疑惑)** |                                                    |    否    |
| 2    | 缺少actual_main_current          | 当前安全控制板主电流                                         |                                                    |    否    |
| 3    | actual_main_voltage              | 当前安全控制板主电压                                         | double getMainVoltage();                           |    是    |
| 4    | actual_robot_voltage             | 当前安全控制板机器人电压                                     | double getRobotVoltage();                          |    是    |
| 5    | actual_robot_current             | 当前安全控制板机器人电流                                     | double getRobotCurrent();                          |    是    |
| 6    | actual_digital_output_bits       | 当前数字输出状态<br>0-7: 标准IO, 8-15: 配置IO<br>**16-17: 工具IO(疑惑)** |                                                    |    否    |
| 7    | operationalModeSelectorInput     | 机器人操作模式选择                                           |                                                    |    否    |
| 8    | threePositionEnablingDeviceInput | 三段使能开关状态                                             |                                                    |    否    |
| 9    | masterboard_temperature          | 主板温度                                                     | double getControlboxTemperature();                 |    是    |
| 10   | analog_io_types                  | 模拟IO类型<br>模拟输入（电流，电压）<br>模拟输出（电流，电压）<br> |                                                    |    否    |
| 11   | 缺少StandardDigitalOutput        |                                                              | std::vector<bool> getStandardDigitalOutput();      |    否    |
| 12   | 缺少StandardDigitalInput         |                                                              | std::vector<bool> getStandardDigitalInput();       |    否    |
| 13   | 缺少ConfigurableDigitalInput     |                                                              | std::vector<bool> getConfigurableDigitalInput();   |    否    |
| 14   | 缺少ConfigurableDigitalOutput    |                                                              | std::vector<bool> getConfigurableDigitalOutput();  |    否    |
| 15   | 缺少ConfigurableAnalogInput      |                                                              | std::vector<double> getConfigurableAnalogInput();  |    否    |
| 16   | 缺少ConfigurableAnalogOutput     |                                                              | std::vector<double> getConfigurableAnalogOutput(); |    否    |
| 17   | standard_analog_input0           | 标准模拟输入电流或者电压                                     | std::vector<double> getStandardAnalogInput();      |    是    |
| 18   | standard_analog_input1           | 标准模拟输入电流或者电压                                     | 同上                                               |    是    |
| 19   | standard_analog_output0          | 标准模拟输出电流或者电压                                     | std::vector<double> getStandardAnalogOutput();     |    是    |
| 20   | standard_analog_output1          | 标准模拟输出电流或者电压                                     | 同上                                               |    是    |
| 21   | master_io_current                | 主机IO电流                                                   | double getMasterIoCurrent();                       |    是    |
| 22   | euromap67_input_bits             | **安全光幕输入位（多少个）**                                 |                                                    |    否    |
| 23   | euromap67_output_bits            | **安全光幕输出位（多少个）**                                 |                                                    |    否    |
| 24   | euromap67_24V_voltage            | 安全光幕24V端电压                                            | double getEuromap67Voltage();                      |    是    |
| 25   | euromap67_24V_current            | 安全光幕24V端电流                                            | double getEuromap67Current();                      |    是    |

#### 工具端

| 序号 | 配方名称                  | 含义                        | 接口                                       | 开放情况 |
| ---- | ------------------------- | --------------------------- | ------------------------------------------ | :------: |
| 1    | tool_mode                 | 工具模式                    |                                            |    否    |
| 2    | tool_output_mode          | 工具输出模式                |                                            |    否    |
| 3    | **缺少**                  | 获取工具端数字输入          | std::vector<bool> getToolDigitalInput();   |    否    |
| 4    | **缺少**                  | 获取工具端数字输出          | std::vector<bool> getToolDigitalOutput();  |    否    |
| 5    | **缺少**                  | 获取工具端模拟量输出        | std::vector<double> getToolAnalogOutput(); |    否    |
| 6    | tool_digital_output0_mode | **工具数字输出0模式**       |                                            |    否    |
| 7    | tool_digital_output1_mode | **工具数字输出1模式**       |                                            |    否    |
| 8    | tool_analog_input_types   | **工具模拟量输入类型**      |                                            |    否    |
| 9    | tool_analog_input0        | 工具模拟量输入0             | std::vector<double> getToolAnalogInput();  |    是    |
| 10   | tool_analog_input1        | 工具模拟量输入1             | 同上                                       |    是    |
| 11   | tool_output_voltage       | 工具输出电压                | double getToolVoltage();                   |    是    |
| 12   | tool_output_current       | 工具输出电流                | double getToolCurrent();                   |    是    |
| 13   | tool_voltage_48V          | **已有tool_output_voltage** |                                            |    否    |
| 14   | tool_current              |                             | double getIoCurrent();                     |    否    |
| 15   | tool_temperature          | 工具端温度                  | double getToolTempture();                  |    是    |
| 16   | actual_tool_accelerometer | 当前工具x,y,z方向加速度值   |                                            |    否    |

#### 寄存器

| 序号 | 配方名称                       | 含义                  | 接口                                           | 开放情况 |
| ---- | ------------------------------ | --------------------- | ---------------------------------------------- | :------: |
| 1    | output_bit_registers_0_to_63   | [000~063]寄存器位输出 | uint64_t getBitOutputRegister0_63();           |    是    |
| 2    | output_bit_registers_64_to_127 | [064~127]寄存器位输出 | uint64_t getBitOutputRegister64_127();         |    是    |
| 3    | output_int_registers           | int型寄存器位输出     | std::vector<uint32_t> getIntOutputRegister() ; |    是    |
| 4    | output_double_registers        | double型寄存器输出    | std::vector<double> getDoubleOutputRegister(); |    是    |
| 5    | input_bit_registers_0_to_63    | [000~063]寄存器位输入 | uint64_t getBitInputRegister0_63();            |    是    |
| 6    | input_bit_registers_64_to_127  | [064~127]寄存器位输入 | uint64_t getBitInputRegister64_127();          |    是    |
| 7    | input_int_registers            | int型寄存器位输入     | std::vector<uint32_t> getIntInputRegister();   |    是    |
| 8    | input_double_registers         | double型寄存器位输入  | std::vector<double> getDoubleInputRegister();  |    是    |

