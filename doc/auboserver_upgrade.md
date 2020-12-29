## 0\. 准备

1\. 系统需要升级到Ubuntu 16.04 x64，实时补丁RT_PREEMPT rt94
2\. 拷贝`release/robot_controller_install_4.10`文件夹到工控机

## 1\. 升级控制器程序

```bash
# 启动
cd robot_controller_install_4.10/RobotServer/bin
sudo ./start.sh
```

注意：
1\. 如果有新的`auboserver`软件释出，需要替换`robot_controller_install_4.10/RobotServer/bin`目录下`auboControllerServer`
2\. 调试时运行`start_debug.sh`

```
# 启动
cd robot_controller_install_4.10/RobotServer/bin
sudo ./start_debug.sh
```

## 2\. 升级接口板固件

```bash
# 切换到固件升级目录
cd robot_controller_install_4.10.0/auboFirmware
# 手动升级接口板Master固件
./firmwareUpgrade 127.0.0.1 ./InterfaceBoardMaster_V3.5.19_aubo_firmware.tar.gz
# 手动升级接口板Slave固件
./firmwareUpgrade 127.0.0.1 ./InterfaceBoardSlave_V3.5.5_aubo_firmware.tar.gz
```

注意：
1\. 升级过程中不要断电  
2\. 升级完成之后重新上电，等到Standby(黄灯)亮起时开机，这会需要稍微等一会
3\. 需要先启动`auboserver`

## 3\. `AuboScope`示教器程序

```bash
cd release/aubo_scope
chmod +x ./aubo_scope*
./aubo_scope*
```

## 4\. `AuboDriver`安装及使用

```bash
cd release/aubo_driver
sudo dpkg -i aubo-driver_?.?.?_amd64.deb
```

