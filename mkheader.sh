#!/bin/sh

# 此脚本生成供调用 EasyCKL 的应用程序使用的头文件
# Copyright daemon_process

# The name is kept EasyCKL.h for backwards compatibility
header_file=SDK/EasyCKL.h
header_file_swp=${header_file}.swp

mkdir -p SDK
rm -rf ${header_file} ${header_file_swp}

files='ec_portable.h browser.h CKLMain.h CustomizeJS.h CustomizeScheme.h EcPackApi.h FrameApi.h'
tags='SimpleHandler CefFrame CefV8Context CefV8ValueList CefRefPtr<CefV8Value> CefRequest BytesVector std::wstring'

for file in $files
do
	cat ${file} >> ${header_file_swp}
done

if [ "$1" = "-d" ] ; then
	cp ${header_file_swp} ${header_file}.native
fi

for tag in $tags
do
	sed -i "s/${tag}/void/g" ${header_file_swp}
done

# 删除 UTF-8 的 BOM(除了 SDK/header.h.begin 中的)
# 必须要先删除 bom，不然下一句就不能彻底清除除 #define 之外的预处理指令了
bom=`echo -ne "\0357\0273\0277"`
sed -i "s/${bom}//g" ${header_file_swp}			# for MinGW
sed -i '/^\xEF\xBB\xBF/d' ${header_file_swp}		# for Linux (Test in Ubuntu 16.04 LTS)

# 删除除 #define 之外的预处理指令
sed -i -e '/^#[^d].*[^\/]$/d' ${header_file_swp}

# 删除 #define __XXXX_H_ 
sed -i -e '/^#define.*_H_$/d' ${header_file_swp}

# 删除 REMOVE IT 标记的行
sed -i -e '/\/\* REMOVE IT \*\/$/d' ${header_file_swp}

# 合并空行
sed -i '/^$/N;/^\n$/D' ${header_file_swp}

cat SDK/header.h.end >> ${header_file_swp}

# SDK/header.h.begin 中包含 bom
cat SDK/header.h.begin > ${header_file}
cat ${header_file_swp} >> ${header_file}
rm -rf ${header_file_swp}

if [ "$1" = "-d" ] ; then
	diff -u ${header_file}.native ${header_file} > ${header_file}.diff
fi
