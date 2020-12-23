#!/bin/bash

###################################################
#
#     frpc停止
#
###################################################

#得到脚本文件的相对路径
script_relative_path="$(dirname "$0")"
echo "this script relative path: $script_relative_path"

#得到脚本文件的绝对路径
script_abs_path=$(cd "$(dirname "$0")/";pwd)
echo "this script abs path: $script_abs_path"

P_ID=$(ps -aux | grep -w "frpc" | grep -v "grep" | awk '{print $2}')
if [ -n "$P_ID" ];then
	kill -9 $P_ID
fi

exit 0
