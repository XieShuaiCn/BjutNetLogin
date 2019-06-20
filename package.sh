#!/bin/sh  
  
exe="BjutNetLogin" #发布的程序名称 
des="../Linux64/" #你的路径  

qthome="${HOME}/.Qt5.9.6/5.9.6/gcc74_64"

cd $des

set PATH=$qthome/lib:$qthome/bin:$qthome/plugins:$PATH

echo 分析依赖关系....
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')  
deplist_2=$(ldd "$qthome/plugins/platforms/libqxcb.so" | awk  '{if (match($3,"/")){ printf("%s "),$3 } }') 
echo 复制依赖库....
echo $deplist
cp -f $deplist ./ 
echo "$qthome/plugins/(bearer|iconengines|imageformats|platforms)"
cp -rf $qthome/plugins/bearer ./
cp -rf $qthome/plugins/iconengines ./
cp -rf $qthome/plugins/imageformats ./
cp -rf $qthome/plugins/platforms ./
echo $deplist_2
cp -f $deplist_2 ./
echo 完成。
