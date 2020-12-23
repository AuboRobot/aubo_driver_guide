#!/bin/bash

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

###################################################
#     frpc启动
###################################################

#脚本文件的相对路径
script_relative_path="$(dirname "$0")"

#脚本文件的绝对路径
script_abs_path=$(cd "$(dirname "$0")/";pwd)

#日志的绝对路径
logfile_abs_path="$script_abs_path/run.log"

W_INFO "script run ..."
W_INFO "script relative path: $script_relative_path"
W_INFO "script abs path: $script_abs_path"

"$script_abs_path/frpc" -c "$script_abs_path/frpc.ini" &


exit 0