#!/bin/bash
###################################################
#
#     配置动态库路径
#
###################################################

#脚本文件的相对路径
script_relative_path="$(dirname "$0")"

#脚本文件的绝对路径
script_abs_path=$(cd "$(dirname "$0")/";pwd)

#程序的安装目录
INSTALL_PATH="/root/AuboRobot"
 
W_INFO "script relative path: $script_relative_path"
W_INFO "script abs path: $script_abs_path"

robot_controller_ld_path="/etc/ld.so.conf.d/"
robot_controller_ld_file="aubo_controller.conf"

echo "$INSTALL_PATH/RobotServer/libs/depends/libmemcached/linux_x64"  > "$robot_controller_ld_path/$robot_controller_ld_file"
echo "$INSTALL_PATH/RobotServer/libs/depends/robotcontrol/linux_x64"  >> "$robot_controller_ld_path/$robot_controller_ld_file"
echo "$INSTALL_PATH/RobotServer/libs/depends/log4cplus/linux_x64/lib" >> "$robot_controller_ld_path/$robot_controller_ld_file"

ldconfig 

exit 0
