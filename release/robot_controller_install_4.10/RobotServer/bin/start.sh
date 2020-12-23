#!/bin/bash

##############################################################################
# function  : console_log
# parameter :
#	 $1 : log_level  eg:"INFO" "DEBUG" "WARN" "ERROR" "FATAL"
#	 $2 : Output information
##############################################################################
function console_log(){

    logfile_path="$logfile_abs_path"

    DATE_N=`date "+%Y-%m-%d %H:%M:%S"`
    USER_N=`whoami`

	if [ $# -eq 2 ] ; then 
		info_level="$1"
		case "$info_level" in
		"INFO")
            echo "${DATE_N} ${USER_N} execute $0 [INFO ] $2"
            echo "${DATE_N} ${USER_N} execute $0 [INFO ] $2" >>"$logfile_path" #执行成功日志打印路径
		;;
		"DEBUG")
            echo "${DATE_N} ${USER_N} execute $0 [DEBUG] $2" 
            echo "${DATE_N} ${USER_N} execute $0 [DEBUG] $2" >>"$logfile_path" #执行成功日志打印路径
        ;;
		"WARN") 
            echo -e "\033[41;37m${DATE_N} ${USER_N} execute $0 [WARN ] $2  \033[0m"
            echo "${DATE_N} ${USER_N} execute $0 [WARN ] $2" >>"$logfile_path" #执行成功日志打印路径
		;;
		"ERROR") 
            echo -e "\033[41;37m${DATE_N} ${USER_N} execute $0 [ERROR] $2  \033[0m"
            echo "${DATE_N} ${USER_N} execute $0 [ERROR] $2" >>"$logfile_path" #执行成功日志打印路径
		;;
		"FATAL") 
            echo -e "\033[41;37m${DATE_N} ${USER_N} [FATAL] $2  \033[0m"
            echo "${DATE_N} ${USER_N} execute $0 [FATAL] $2" >>"$logfile_path" #执行成功日志打印路径
		;;
		esac
	fi
}

function W_INFO(){
    console_log "INFO" "$@"
}

function W_DEBUG(){
    console_log "DEBUG" "$@"
}

function W_WARN(){
    console_log "WARN" "$@"
}

function W_ERROR(){
    console_log "ERROR" "$@"
}

function W_FATAL(){
    console_log "FATAL" "$@"
}

############################################################################
# function  : main
############################################################################

#脚本文件的相对路径
script_relative_path="$(dirname "$0")"

#脚本文件的绝对路径
script_abs_path=$(cd "$(dirname "$0")/";pwd)

#日志的绝对路径
logfile_abs_path="$script_abs_path/run.log"

if [ -f "$logfile_abs_path" ];then
    file_row_count=$(cat run.log |wc -l)
    if [ $file_row_count -gt 30 ];then
        rm -rf "$logfile_abs_path"
        W_INFO "remove old log"
    fi
fi

W_INFO "script run ..."
W_INFO "script relative path: $script_relative_path"
W_INFO "script abs path: $script_abs_path"


#ps -ef |grep auboControllerServer 这个就是看auboControllerServer的启动情况
#grep -v "grep" 是为了去掉查询 grep的那一条
#wc -l 是计数的

COUNT=$(ps -ef |grep auboControllerServer |grep -v "grep" |wc -l)
if [ $COUNT -gt 0 ]; then
    W_WARN "auboControllerServer already running"
    exit 0
fi

#声明动态库的路径
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$script_abs_path/../libs/depends/libmemcached/linux_x64/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$script_abs_path/../libs/depends/log4cplus/linux_x64/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$script_abs_path/../libs/depends/robotcontrol/linux_x64/

cd "$script_abs_path"
nohup ./auboControllerServer | ./robotlog4 &
cd $OLDPWD

port=8899
COUNT=$(ps -ef |grep auboControllerServer |grep -v "grep" |wc -l)
if [ $COUNT -gt 0 ]; then
    W_INFO "auboControllerServer running ... "
    for i in {1..15}
    do
        TCPListeningnum=`netstat -an | egrep ":${port}" | awk '$1 == "tcp" && $NF == "LISTEN" {print $0}' |wc -l`
        #UDPListeningnum=`netstat -an | egrep ":${port}" | awk '$1 == "udp" && $NF == "0.0.0.0:*" {print $0}' |wc -l`
        if [ $TCPListeningnum -eq 0 ];then
            sleep 1
        else
            W_INFO "$port LISTEN, auboControllerServer start finish."
            break
        fi

        if [ $i -eq 30 ];then
            W_ERROR "auboControllerServer start failed"
        fi
    done
fi

exit 0
