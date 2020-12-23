#!/bin/bash
###################################################
#
#     sn文件编辑
#
###################################################

#得到脚本文件的相对路径
script_relative_path="$(dirname "$0")"
echo "this script relative path: $script_relative_path"

#得到脚本文件的绝对路径
script_abs_path=$(cd "$(dirname "$0")/";pwd)
echo "this script abs path: $script_abs_path"

SERVER_NAME="agentMachine"

while true; do
	count=`ps -fe |grep "$SERVER_NAME" | grep -v "grep"`
	
	if [ "$?" != "0" ]; then
        	echo ">>>>no $SERVER_NAME,run it"
        	"$script_abs_path/$SERVER_NAME"
	else
        	echo ">>>>$SERVER_NAME is running"
	fi
   
   sleep 2
   
done &
