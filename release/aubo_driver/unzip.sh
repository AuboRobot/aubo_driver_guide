#! /bin/bash

echo $1
# 判断传入参数
if [ ! $1 ];then
	echo "请在脚本文件后指定要解压的文件！"
	exit 1    
fi

Zip_DIR=$1
File_DIR=$PWD/${Zip_DIR:0:${#Zip_DIR}-4}

# 判断指定文件是否存在 
if [ ! -f $Zip_DIR ];then
	echo "压缩文件不存在！"
	exit 1   
fi

# 解压
unzip -o $Zip_DIR           #覆盖式解压
if [ $? == 1 ]                                                                
then                                                                          
    echo "解压失败,请检查是否安装unzip工具"                                                                 
    exit 1                                                                    
fi     
cd    $File_DIR

#删除旧头文件和库文件(链接符)
if [ -L "/usr/arcs/lib/libaubo_driver*" ];then
  sudo rm /usr/arcs/lib/libaubo_driver*
else
  if [ ! -d "/usr/arcs/" ];then
  	sudo mkdir /usr/arcs/
  fi
  if [ ! -d "/usr/arcs/lib" ];then
  	sudo mkdir /usr/arcs/lib
  fi
fi

if [ -d "/usr/arcs/include/aubo_driver/" ];then
  sudo rm -rf /usr/arcs/include/aubo_driver
else
  if [ ! -d "/usr/arcs/" ];then
  	sudo mkdir /usr/arcs/
  fi
  if [ ! -d "/usr/arcs/include" ];then
  	sudo mkdir /usr/arcs/include
  fi
fi


#拷贝新头文件和库文件
sudo cp $PWD/lib/libaubo_driver* /usr/arcs/lib/
sudo cp -rf $PWD/include/aubo_driver/ /usr/arcs/include/

echo "install successful!"
