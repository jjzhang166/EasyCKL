#!bash

#此脚本帮助你查找当前目录下的所有obj文件并把他们用空格间隔输出到标准输出

# Copyright daemon_process

files=`ls`

for file in $files
do
	if [ "${file##*.}" = "obj" ] ; then
		echo -n "$file "
	fi
done