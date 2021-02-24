## 0\. 准备

1\. 系统需要升级到Ubuntu 16.04 x64，实时补丁RT_PREEMPT rt94
2\. 拷贝`release/aubo_server`文件夹到工控机

## 1\. 升级控制器程序

```bash
# 启动，建议使用最新版本
cd aubo_server
chmod a+x ./auboControllerServer*
sudo ./auboControllerServer*
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
3\. 需要先启动`auboControllerServer`

## 3\. `AuboScope`示教器程序

```bash
cd release/aubo_scope
chmod a+x ./aubo_scope*
./aubo_scope*
```

## 4\. `AuboDriver`安装及使用

##### 版本号说明

​	主版本号.次版本号.修订号	

​	主版本号(MAJOR version)：出现不兼容的API变化
​	次版本号(MINOR version)：新增向后兼容的功能
​	修订号(补丁版本号，PATCH version)：修复向后兼容的bug安装方式

​    **要注意：主版本号为0的时候，接口函数可能会变化的风险，容易发生不兼容情况。**

##### 安装

 将 aubo-driver_?.?.?.zip 下载到release/aubo_driver目录下

```bash
cd release/aubo_driver
chmod u+x unzip.sh
sudo ./unzip.sh aubo-driver_?.?.?.zip

```



