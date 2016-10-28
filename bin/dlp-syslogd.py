#!/usr/bin/python
#-*- coding: utf-8 -*-

import json
import sys
import socket
import re


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
		self.protol = protol
		with open(rulefile,'r') as f:
			self.data = json.load(f)
	

class HttpMatcher(DataMatchTool):
	uri_rule_list=[]
	html_rule_list=[]
	xml_rule_list=[]
	def __init__(self,rulefile):
		DataMatchTool.__init__(self,rulefile, "http")
		uri_array = self.data["rules"]["uri"]
		for m in uri_array:
			uri_json= json.loads(json.dumps(m))
			self.uri_rule_list.append(uri_json)
		html_array = self.data["rules"]["html"]
		for m in html_array:
			html_json= json.loads(json.dumps(m))
			self.html_rule_list.append(html_json)
		xml_array = self.data["rules"]["xml"]
		for m in xml_array:
			xml_json= json.loads(json.dumps(m))
			self.xml_rule_list.append(xml_json)

	def printRuleList(self):
		print "------------uri---------------------"
		for i in range(len(self.uri_rule_list)):
			print "rule ",i+1,"key:",self.uri_rule_list[i]["key"],"msg:",self.uri_rule_list[i]["msg"]
		print "------------html---------------------"
		for i in range(len(self.html_rule_list)):
			print "rule ",i+1,"key:",self.html_rule_list[i]["key"],"msg:",self.html_rule_list[i]["msg"]
		print "------------xml---------------------"
		for i in range(len(self.xml_rule_list)):
			print "rule ",i+1,"key:",self.xml_rule_list[i]["key"],"msg:",self.xml_rule_list[i]["msg"]

	def matchURI(self,URI):
		for i in range(len(self.uri_rule_list)):
			if URI.find(self.uri_rule_list[i]["key"]) >= 0:
				return self.uri_rule_list[i]
	def matchHTML(self,htmlfile):
		with open(htmlfile,'r') as f:
			str = f.read()	
			for i in range(len(self.html_rule_list)):
				match = re.search(self.html_rule_list[i]["key"], str)
				if match :
					return self.html_rule_list
	def matchXML(self,xmlfile):
		with open(xmlfile,'r') as f:
			str = f.read()	
			for i in range(len(self.xml_rule_list)):
				match = re.search(self.xml_rule_list[i]["key"], str)
				if match :
					return self.xml_rule_list

#########################################################




#########################################################
#unix socket






if __name__ == "__main__":
	m_httpmatcher = HttpMatcher(http_config_file)
	# m_httpmatcher.printRuleList()
	print m_httpmatcher.matchURI("Userlogin")
	print type(m_httpmatcher.matchURI("login"))

	print m_httpmatcher.matchHTML("sslvpn.html")
