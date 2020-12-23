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
# function  : returns a string that has whitespace removed
#             from the start and the end,
#             and that has each sequence of internal whitespace
#             replaced with a single space.
# parameter :
#	 $1 : string
# return    : string
############################################################################
function simplified(){
    echo "$(echo "$1" | sed 's/^[[:space:]]*//' | sed 's/[[:space:]]*$//' \
    | sed 's/[[:space:]][[:space:]]*/ /g')"

    return 0
}

############################################################################
# function  : check is absolute path
# parameter :
#	 $1 : path string
# output parameter :
#     true : is absolute path
#     other: not absolute path
# return    :
#         0 : exec succ
#     other : exec have error
############################################################################
function is_absolute_path(){
    
    #入口判断
    if [ $# -ne 1 ];then
        W_ERROR "is_absolute_path() missing parameters."
        return 1
    fi

    if [ "$(echo "$1" | cut -c 1-1)" = '/' ];then
        echo "true"
    else
        echo "false"
    fi
    return 0
}

############################################################################
# function  : give executable permissions
# parameter :
#	 $1 : absolute path
# return    :
#         0 : success
#         1 : failure, $1 is not a directory
############################################################################
function give_executable_permissions(){
    
    ret=0
    #判断参数个数是否==1
    if [ $# -ne 1 ];then
        ret=1
        W_ERROR "exec give_executable_permissions() missing parameters. count=$#"
        return $ret
    fi

    #判断源文件路径是否为绝对路径
    isAbsolutePath=$(is_absolute_path "$1")
    if [ $? -eq 0 ]; then
        if [ "$isAbsolutePath" != "true" ];then
            ret=1
            W_ERROR "exec give_executable_permissions() parameters not absolute path"
            return $ret
        fi
    else
        ret=1
        W_ERROR "exec give_executable_permissions() exec is_absolute_path() failed"
        return $ret
    fi

    if [ ! -f "$1" ];then
        ret=1
        W_ERROR "give_executable_permissions() parameters file not exist. "
        return $ret
    fi

    #文件存在 且 并不具有可执行权限
    if [ -f "$1" -a ! -x "$1" ];then
        chmod +x "$1"
        ret=$?
    fi
    return $ret
}

############################################################################
# function  : execute shell script
# parameter :
#	 $1 : shell script path
#	 $2 : workspace path
# return    :
#         0 : success
#     other : failure
############################################################################
function script_exec(){
    ret=0
    if [ $# -ne 1 -o -f "$1" ];then
        give_executable_permissions "$1"
        cmd="$1"
        ${cmd}
        ret=$?
    else
        ret=1
        W_ERROR "script_exec() parameters file not exist. "
    fi
    return $ret
}

############################################################################
# function  : read conf file
# parameter :
#	 $1 : conf file path
# return    :
#         0 : success
#     other : failure
############################################################################
function read_conf(){
    
    ret=0
    #判断参数个数是否==1
    if [ $# -ne 1 ];then
        ret=1
        W_ERROR "exec read_conf() missing parameters. count=$#"
        return $ret
    fi

    if [ ! -f "$1" ];then
        ret=1
        W_ERROR "read_conf() $1 does not exist"
        return $ret
    fi

    #单行遍历读取配置文件
    while read -r line
    do
        # ${#line}  字符串长度
        # ignore empty strings  空串
        if [ ${#line} -lt 2 ];then
            continue
        fi

        # ignore comments  注释
        echo "$line" | grep -E '^#' >/dev/null
        if [ $? -eq 0 ]; then
            #W_INFO "注释:$line"
            continue
        fi

        W_INFO "config item: $line"

        conf_item=$(simplified "$line")
        parsing_conf_info "$conf_item"
    done < "$1"

    return $ret
}

############################################################################
# function  : Parsing command item
# parameter :
# return    :
#         0 : success
#     other : failure
############################################################################
function parsing_conf_info(){
    
    conf_item="$1"
    operation_type="${conf_item%% *}"
    content="${conf_item#* }"

    #W_INFO "操作类型：$operation_type  内容：$content"
    case $operation_type in
    EXEC)
        handle_EXEC_cmd "$content"
    ;;
    *)
        W_WARN "unknown command type"
    ;;
    esac
}

function handle_EXEC_cmd(){

    ret=0
    workspace_path="$WORKSPACE_PATH"

    #判断源文件路径是否为绝对路径
    isAbsolutePath=$(is_absolute_path "$1")
    if [ $? -ne 0 ]; then
        ret=1
        W_ERROR "exec handle_CLEDIR_cmd() exec is_absolute_path() failed"
        return $ret
    fi

    if [ "$isAbsolutePath" == "true" ];then
        #绝对路径
        exec_path="$1"
    else
        #相对路径
        exec_path="$workspace_path/$1"
    fi
    
    script_exec "$exec_path"
    ret=$?
    
    if [ $? -eq 0 ];then
        W_INFO "exec($exec_path) succ"
    fi

    return $ret
}

############################################################################
# 
#function  : main
#
############################################################################

#脚本文件的相对路径
script_relative_path="$(dirname "$0")"

#脚本文件的绝对路径
script_abs_path=$(cd "$(dirname "$0")/";pwd)

#工作目录
WORKSPACE_PATH=$(cd "$script_abs_path/../";pwd)

#日志的绝对路径
logfile_abs_path="$script_abs_path/run.log"

#安装配置文件
STARTUP_CONF_PATH="$script_abs_path/startup.list"

#程序参数数量
W_INFO "script relative path: $script_relative_path"
W_INFO "script abs path: $script_abs_path"
W_INFO "WORKSPACE_PATH: $WORKSPACE_PATH"
W_INFO "STARTUP_CONF_PATH: $STARTUP_CONF_PATH"


#读取并解析配置文件
read_conf "$STARTUP_CONF_PATH"

#必要的系统命令
sync

dateInfo=$(date "+%Y-%m-%d %H:%M:%S")
echo "Install result : $dateInfo succ" >"$WORKSPACE_PATH/version_update_ret"