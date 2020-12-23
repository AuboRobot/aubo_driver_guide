#!/bin/bash

#########################################################
#
#[common]
#server_addr = 39.96.22.112
#server_port = 7000

#[hanwei-robot-6012]
#type = tcp
#local_ip = 127.0.0.1
#local_port = 22
#remote_port = 6012
#
#########################################################


#得到脚本文件的相对路径
script_relative_path="$(dirname "$0")"
echo "this script relative path: $script_relative_path"

#得到脚本文件的绝对路径
script_abs_path=$(cd "$(dirname "$0")/";pwd)
echo "this script abs path: $script_abs_path"

frpc_config_file="/root/AuboRobot/services/frp/frpc.ini"

#机械臂SN信息
sn="$1"
sn=$(cat /root/.sn |grep "sn" | awk -F '"' '{print $2}')
echo "ROBOT SN=$sn"

#取得具体的SN码
declare -i sn_num=10#${sn:2}

#远程服务器IP+PORT
server_addr="39.96.22.112"
server_port="7000"

#需要编辑 name remote_port

#远程端口
declare -i remote_port=13000+$sn_num
name="[$sn-$remote_port]"
type="tcp"
local_ip="127.0.0.1"
local_port="22"

echo "remote_port=$remote_port"
echo "name=$name"

#编辑追溯文件
if [ -n "$server_addr" -a -n "$server_port" -a -n "$remote_port" -a -n "$name" -a -n "$type" -a -n "$local_ip"  -a -n "$local_port" ];then
	
	echo "" >  "$frpc_config_file"
	
	echo "[common]" >>  "$frpc_config_file"
	echo "server_addr = $server_addr" >>  "$frpc_config_file"
	echo "server_port = $server_port" >>  "$frpc_config_file"
	
	echo "" >>  "$frpc_config_file"
	
	echo "$name" >>  "$frpc_config_file"
	echo "type = tcp" >>  "$frpc_config_file"
	echo "local_ip = $local_ip" >>  "$frpc_config_file"
	echo "local_port = $local_port" >>  "$frpc_config_file"
	echo "remote_port = $remote_port" >>  "$frpc_config_file"
	
	echo "succ"
else
		
	if [ -z "$version" ];then
		echo "ERROR:version is null"
	fi
	if [ -z "$sn" ];then
		echo "ERROR:sn is null"
	fi
	if [ -z "$workorder"  ];then
		echo "ERROR:workorder is null"
	fi
	if [ -z "$powerinfo" ];then
		echo "ERROR:powerinfo is null"
	fi
fi

exit 0
