���裺
1.���밲װ
	tar jxvf xxx.tar.bz2
	cd webhelper-xxxx
	make
	make install (��ʧ��,����ִ�� webhelper -q)
	
2.���ÿ���������
	vi /etc/rc.local
	���: /etc/webhelper.d/webhelper_cron.sh
	
3.�����ػ�������־: 
	vi /etc/syslog.conf
	�޸�: local2.none ��ӵ�/var/log/messages�������棬�����Ͳ����ظ��İ���־��ӵ�messages
	���: local2.*				/var/log/webhelper.log
	/etc/init.d/syslog restart
	chattr +a /var/log/webhelper.log

4.������־�и�
vi /etc/logrotate.d/webhelper  (����¹���)
��������:
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

	���ú�ʹ����Ч
	logrotate -f /etc/logrotate.conf
	�Ժ��ÿ�찴ʱ�и���־

5.��������
����sock������Ϊ10240
ulimit -n 10240
��������ӵ�/etc/profile
	
6.�����˳��Զ�����
	crontab -e -u root
	* * * * * /etc/webhelper.d/webhelper_cron.sh

7.�鿴������־
	��־��/var/log/Ŀ¼��,webhelper.log�ǵ�����־,webhelper.log-20130601���������־

8.�鿴����������־
	cat /etc/webhelper.d/start.log

9.�鿴get��postʧ�ܵ�html�ļ�
	��/var/log/webhelper/Ŀ¼�£���key����


����Ŀ�ѡ��:
-h					�鿴����
-t num			�����̳߳�������Ĭ��20
-i ip1,ip2	����ָ��N��(N>=1)��֤��ʶ�������ip���Զ��ŷָ�����Ϊ���÷�����
-p port			������֤��ʶ��������˿ں�
-d					ָ��Ϊ�ػ��������У���־������/var/log/webhelper.log
-q					��ȫ���˳��ó���

����:
webhelper -i 192.168.1.22 -p 26128 -t 50 -d
webhelper -q  //�˳��ػ�����