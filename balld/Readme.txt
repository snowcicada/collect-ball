步骤：
1.编译安装
	tar jxvf xxx.tar.bz2
	cd webhelper-xxxx
	make
	make install (若失败,则先执行 webhelper -q)
	
2.配置开机启动：
	vi /etc/rc.local
	添加: /etc/webhelper.d/webhelper_cron.sh
	
3.配置守护进程日志: 
	vi /etc/syslog.conf
	修改: local2.none 添加到/var/log/messages规则里面，这样就不会重复的把日志添加到messages
	添加: local2.*				/var/log/webhelper.log
	/etc/init.d/syslog restart
	chattr +a /var/log/webhelper.log

4.配置日志切割
vi /etc/logrotate.d/webhelper  (添加新规则)
内容如下:
/var/log/webhelper.log {
  daily
  dateext
  rotate 7
  sharedscripts
  prerotate
      /usr/bin/chattr -a /var/log/webhelper.log
  endscript
  sharedscripts
  postrotate
      /usr/bin/killall -HUP syslogd
      /usr/bin/chattr +a /var/log/webhelper.log
  endscript
}

	配置后使其生效
	logrotate -f /etc/logrotate.conf
	以后就每天按时切割日志

5.设置上限
设置sock连接数为10240
ulimit -n 10240
将这行添加到/etc/profile
	
6.程序退出自动重启
	crontab -e -u root
	* * * * * /etc/webhelper.d/webhelper_cron.sh

7.查看程序日志
	日志在/var/log/目录下,webhelper.log是当天日志,webhelper.log-20130601七天类的日志

8.查看程序重启日志
	cat /etc/webhelper.d/start.log

9.查看get和post失败的html文件
	在/var/log/webhelper/目录下，以key命名


程序的可选项:
-h					查看帮助
-t num			设置线程池数量，默认20
-i ip1,ip2	可以指定N个(N>=1)验证码识别服务器ip，以逗号分隔，作为备用服务器
-p port			设置验证码识别服务器端口号
-d					指定为守护进程运行，日志保存在/var/log/webhelper.log
-q					安全的退出该程序

例子:
webhelper -i 192.168.1.22 -p 26128 -t 50 -d
webhelper -q  //退出守护进程