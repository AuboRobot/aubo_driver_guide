#!/bin/bash

###################################################
#
#     系统优化
#
###################################################

#得到脚本文件的相对路径
script_relative_path="$(dirname "$0")"
echo "this script relative path: $script_relative_path"

#得到脚本文件的绝对路径
script_abs_path=$(cd "$(dirname "$0")/";pwd)
echo "this script abs path: $script_abs_path"

#保存的数量
declare -i save_count=5

#待优化的文件夹
work_path="/root/.aubo_versions/"

cd "$work_path"

#计算文件夹的数量
declare -i dir_count=$(ls -l |grep "^d"|wc -l)

if [ "$dir_count" -gt "$save_count" ];then
	
	cd "$work_path"
	
	declare -i to_be_deleted_count=$dir_count-$save_count
	
	ls -lrt | grep "^d" | awk '{print $9}' | head -n "$to_be_deleted_count" | xargs rm -rf 
fi

exit 0

