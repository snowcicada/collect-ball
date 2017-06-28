#!/bin/sh

num=`ps -eLf | grep webhelper | wc -l`
if [ $num -lt 10 ];then
	echo `date +"%Y-%m-%d %H:%M:%S"` start webhelper >> /etc/webhelper.d/start.log
	webhelper -i 202.39.151.118,202.153.198.69 -p 26128 -t 50 -d
fi
