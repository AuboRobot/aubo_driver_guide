#!/bin/bash

###########################################################
# 筛除 接口板网卡 + 无线网卡 + lo
#
# 功能: 设置用户以太网口的静态IP 
#     ./XXX.sh -h 192.168.33.232 -m 255.255.255.0 -g 192.168.33.1
###########################################################

#得到脚本文件的相对路径
script_relative_path="$(dirname "$0")"
echo "this script relative path: $script_relative_path"

#得到脚本文件的绝对路径
script_abs_path=$(cd "$(dirname "$0")/";pwd)
echo "this script abs path: $script_abs_path"

#网卡的系统路径
sys_net_dir="/sys/class/net/"

#网络配置文件
network_conf_file="/etc/network/interfaces"

#静态IP
static_ip="192.168.33.232"

#子网掩码
netmask="255.255.255.0"

#网关
gateway=""

#用户网卡名称
user_network_name=""

#解析命令行参数
while getopts ":h:m:g:" opt
do
    case $opt in
        h)
            static_ip="$OPTARG"
            echo "static_ip:$OPTARG"
        ;;
        m)
            netmask="$OPTARG"
            echo "netmask:$OPTARG"
        ;;
        g)
			gateway="$OPTARG"
            echo "gateway:$OPTARG"
        ;;
        *)
            echo "未知参数$opt"
            echo "参数值$OPTARG"
        ;;
        ?)
            echo "未知参数$opt"
            echo "参数值$OPTARG"
        ;;
    esac
done


#执行"寻找接口板"程序  输出信息到指定文件
$script_abs_path/findInterfaceBoard > "$script_abs_path/findInterfaceBoard.log"

#接口板对应的网卡
interfaceboard_network_name=`grep -a "interfaceboard:" $script_abs_path/findInterfaceBoard.log | tail -n 1 | awk -F "interfaceboard:" '{print $2}'`

if [ -n "$interfaceboard_network_name" ]; then
	if [ -d "$sys_net_dir/$interfaceboard_network_name" ]; then
		echo "连接接口板的网卡: $interfaceboard_network_name"
	else
		echo "==================================="
		echo "ERORR:系统中未发现 接口板配置对应的网卡信息"
		echo "==================================="
		exit 1
	fi
else
	echo "==================================="
	echo "ERORR:配置文件中未发现接口板对应网卡配置"
	echo "==================================="
	exit 1
fi

#筛查网卡列表  得到用户对应的网卡信息
ifnames=$(ls $sys_net_dir | grep -v ^lo | grep -v "$interfaceboard_network_name")
for ifname in ${ifnames}
do 
	if [ ! -d "$sys_net_dir/$ifname/wireless" ]; then
		user_network_name="$ifname"
		break
	fi
done

#设置网卡属性
if [ -n "$user_network_name" ]; then
	
	echo "用户网卡:$user_network_name"
	
    #重写网卡配置文件
	echo "auto lo" >  "$network_conf_file"
	echo "iface lo inet loopback" >> "$network_conf_file"
	echo "" >> "$network_conf_file"

	echo "auto $user_network_name" >> "$network_conf_file"
	echo "iface $user_network_name inet static" >> "$network_conf_file"
	echo "address $static_ip" >> "$network_conf_file"
	echo "netmask $netmask"  >> "$network_conf_file"
	if [ -n "$gateway" ];then
		echo "gateway $gateway" >> "$network_conf_file"
	fi
	
	#重启服务
	$(ip addr flush dev "$user_network_name")
	$(ifdown "$user_network_name")
	$(ifup "$user_network_name")

	#service networking restart
	
	echo "==================================="
	echo "SUCC 设置静态IP并重启网络成功"
	echo "==================================="
else
	echo "==================================="
	echo "ERORR:未找到供用户使用的有线网卡"
	echo "==================================="
fi

exit 0
