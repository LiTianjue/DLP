#!/usr/bin/python
#-*- coding: utf-8 -*-

import json
import sys
import socket


print "开始"




work_dir = "/home/andy/GitHub/WORK/DLP/"
dlp_config_file=work_dir+"etc/dlp-syslogd.json"
http_config_file=work_dir+"etc/protocol/http.json"



# 读取json格式打主配置文件，获取unix-socket通讯地址，syslog服务器地址，要读取的匹配规则
#########################################################
class JsonHelper:
	def __init__(self,filename):
		self.file = filename 
		with open(filename,'r') as f:
			self.json_data=json.load(f)

	def getTopElement(self,name):
		return self.json_data[name]


#########################################################



#########################################################
# 用于将日志组织为json格式
class LogHelper:
	logmsgdict = {}
	def __init__(self,log_type):
		pass
	def AddElement(self,name,value):
		self.logmsgdict[name] = value
	def clearElement(self):
		self.logmsgdict.clear()
	def getJsonOutput(self):
		return  json.dumps(self.logmsgdict)

#########################################################




#########################################################
# 用于发送日志类
class LogSender:
	logip = "0.0.0.0"
	logport = 514
	facility = 0

	def __init__(self,ip,port):
 		self.addr = (ip,port)
		self.sock= socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	def __del__(self):
		self.sock.close()
	def  log(self,msg,level):
 		logline = '<%d> : %s' % (level+self.facility,msg)
		self.sock.sendto(logline,self.addr)

#########################################################




#########################################################
# unix-socket 类，用于进程之间进行通讯


#########################################################



#########################################################
# 数据匹配类，用于对数据信息进行比对和匹配
class DataMatchTool:
	protol = "undefine"
	def __init__(self,rulefile,protol):
		self.protol = prontRuleList()
	

