#!/bin/bash
###################################################
#
#     sn文件编辑
#
###################################################


#判断参数是否正确
if [ $# != 2 ] ; then
	echo " USAGE: param error"
	echo " e.g.: $0 sn workorder"
	exit 1;
fi


#得到脚本文件的相对路径
script_relative_path="$(dirname "$0")"
echo "this script relative path: $script_relative_path"

#得到脚本文件的绝对路径
script_abs_path=$(cd "$(dirname "$0")/";pwd)
echo "this script abs path: $script_abs_path"

sn_file="/root/.sn"

version="V1.0.0"
sn=""
workorder=""
powerinfo="1000W"

sn="$1"
workorder="$2"

if [ -f "$sn_file" ];then
	#删除原先的追溯文件
	rm -rf "$sn_file"
fi


#编辑追溯文件
if [ -n "$version" -a -n "$sn" -a -n "$workorder" -a -n "$powerinfo" ];then
	echo "version = \"$version\"; " >  "$sn_file"
	echo "" >>  "$sn_file"

	echo "sn = \"$sn\"; " >>  "$sn_file"
	echo "workorder = \"$workorder\"; " >>  "$sn_file"
	echo "powerinfo = \"$powerinfo\"; " >>  "$sn_file"
	echo "" >>  "$sn_file"
	chmod 444 "$sn_file"

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
