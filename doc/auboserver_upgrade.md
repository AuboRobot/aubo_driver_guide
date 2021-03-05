## 0\. 准备

1\. 系统需要升级到Ubuntu 16.04 x64，实时补丁RT_PREEMPT rt94, 从release文件中下载：

​	linux-headers-4.14.195-rt94_4.14.195-rt94-1_amd64.deb

​	linux-image-4.14.195-rt94_4.14.195-rt94-1_amd64.deb  

​    打补丁操作：

```bash
dpkg -i linux-headers-4.14.195-rt94_4.14.195-rt94-1_amd64.deb
dpkg -i linux-image-4.14.195-rt94_4.14.195-rt94-1_amd64.deb  
```

​    重启后，查看是否成功(打印文字是否有rt字样)：

```
uname -a
```

2.release文件下载：AuboController,   robot_controller_install,   aubo_driver,   aubo_scope.

##   

## 1\. 升级控制器程序(控制柜端启动)

克隆项目：

```bash
git clone https://github.com/AuboRobot/aubo_driver_guide.git
cd aubo_driver_guide
```

将准备工作中下载到的文件拷贝到 install 文件夹下

提示：克隆使用链接https://gitclone.com/github.com/AuboRobot/aubo_driver_guide.git速度会加快

从终端进入到 相应版本的 AuboController 路径下

```
cd bin/
chmod a+x ./auboControllerServer*
sudo ./auboControllerServer*
```



## 2\. 升级接口板固件(控制柜端启动)

从终端进入到 相应版本的 robot_controller_install 路径下

```
# 切换到固件升级目录
cd auboFirmware/
# 手动升级接口板Master固件
./firmwareUpgrade 127.0.0.1 ./InterfaceBoardMaster_V3.5.19_aubo_firmware.tar.gz
# 手动升级接口板Slave固件
./firmwareUpgrade 127.0.0.1 ./InterfaceBoardSlave_V3.5.5_aubo_firmware.tar.gz
```

注意：
1\. 升级过程中不要断电  
2\. 升级完成之后重新上电，等到Standby(黄灯)亮起时开机，这会需要稍微等一会
3\. 需要先启动`auboControllerServer`



## 3\. `AuboScope`示教器程序(用户端启动)

从终端进入到 相应版本的 aubo_scope 路径下

```
chmod a+x ./aubo_scope*
./aubo_scope*
```



## 4\. `AuboDriver`安装及使用(用户端启动)

##### 版本号说明

​	主版本号.次版本号.修订号	

​	主版本号(MAJOR version)：出现不兼容的API变化
​	次版本号(MINOR version)：新增向后兼容的功能
​	修订号(补丁版本号，PATCH version)：修复向后兼容的bug安装方式

​    **要注意：主版本号为0的时候，接口函数可能会变化的风险，容易发生不兼容情况。**

##### 安装

​    从终端回到 aubo_driver_guide 路径下

```
cd install/
chmod u+x deploy.sh
sudo ./deploy.sh aubo-driver_?.?.?.zip
```



