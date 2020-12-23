#!/bin/bash

###################################################
#
#     中断设置
#
###################################################


#得到脚本文件的相对路径
script_relative_path="$(dirname "$0")"
echo "this script relative path: $script_relative_path"

#得到脚本文件的绝对路径
script_abs_path=$(cd "$(dirname "$0")/";pwd)
echo "this script abs path: $script_abs_path"

#网卡的系统路径
sys_net_dir="/sys/class/net/"

#用户网卡名称
interfaceboard_network_name=""

#执行"寻找接口板"程序  输出信息到指定文件
$script_abs_path/findInterfaceBoard > "$script_abs_path/findInterfaceBoard.log"

#接口板对应的网卡
interfaceboard_network_name=`grep -a "interfaceboard:" $script_abs_path/findInterfaceBoard.log | tail -n 1 | awk -F "interfaceboard:" '{print $2}'`

if [ -n "$interfaceboard_network_name" ]; then
	if [ -d "$sys_net_dir/$interfaceboard_network_name" ]; then
		echo "连接接口板的网卡: $interfaceboard_network_name"
	else
		echo "系统中未发现 接口板配置对应的网卡信息"
		exit 1
	fi
else
	echo "配置文件中未发现接口板对应网卡配置"
	exit 1
fi


#set ALL smp_affinity value
arg=$(cat /proc/interrupts | awk -F ':' '{print $1}');
for i in $arg;
do	
	if [ -f "/proc/irq/$i/smp_affinity" ];then
		echo "B" > /proc/irq/$i/smp_affinity
	fi
done

arg=$(cat /proc/interrupts |grep -e "$interfaceboard_network_name" | awk -F ':' '{print $1}');
for i in $arg;
do	
	if [ -f "/proc/irq/$i/smp_affinity" ];then
		#echo $i "select"
		echo "4" > /proc/irq/$i/smp_affinity
	fi
done

dateInfo=$(date "+%Y-%m-%d %H:%M:%S")
echo "interrupts setting time : $dateInfo" > "$script_abs_path/default.txt"
