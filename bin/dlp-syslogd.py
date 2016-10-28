#!/usr/bin/python
#-*- coding: utf-8 -*-

import json
import sys
import socket
import re
import os
import threading


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
g_cfg_helper = JsonHelper(dlp_config_file)


#########################################################
# 用于将日志组织为json格式
class LogHelper:
	logmsgdict = {}
	def __init__(self,log_type):
		pass
	def AddElement(self,name,value):
		self.logmsgdict[name] = value
	def DelElement(self,name):
		del self.logmsgdict[name]
	def GetElement(self,name):
		return self.logmsgdict[name]
	def clearElement(self):
		self.logmsgdict.clear()
	def getJsonOutput(self):
		return  json.dumps(self.logmsgdict)
	def updateDict(self,data):
		self.logmsgdict.update(data)



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

g_logsender = LogSender(g_cfg_helper.getTopElement("syslog-ip"), int(g_cfg_helper.getTopElement("syslog-port")))


#########################################################
# 数据匹配类，用于对数据信息进行比对和匹配
class DataMatchTool:
	protol = "undefine"
	def __init__(self,rulefile,protol):
		self.protol = protol
		with open(rulefile,'r') as f:
			undecoddata = json.dumps(json.load(f))
			self.data = json.loads(undecoddata.decode("utf-8"))
	

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
					return self.html_rule_list[i]
	def matchXML(self,xmlfile):
		with open(xmlfile,'r') as f:
			str = f.read()	
			for i in range(len(self.xml_rule_list)):
				match = re.search(self.xml_rule_list[i]["key"], str)
				if match :
					return self.xml_rule_list[i]
	def matchALlType(self,source,type):
		if type=="uri":
			return self.matchURI(source)
		elif type=="html":
			return self.matchHTML(source)
		elif type=="xml":
			return self.matchXML(source)

#########################################################
g_httpMatcher = HttpMatcher(http_config_file)

###########################################################


#########################################################
#unix socket
dlp_socket = "/tmp/dlp.sock"

class SyslogServer:
	def __init__(self,socket_name):
		self.sock_file = socket_name
		self.server_socket = socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)
		self.server_socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
		self.server_socket.bind(socket_name)
		self.server_socket.listen(1)

	def handler(self,connection):
		while True:
			data = connection.recv(1024)
			if not data:
				return False
			else:
				print data
			jdata = json.loads(data)
			print type(jdata)
			self.handlerData(jdata)

	def handlerData(self,data):
		logtype = data["protocol"]
		logrule = data["rule"]
		logsource = data["source"]
		del data["source"]
		logdata = LogHelper(logtype)
		logdata.updateDict(data)
		if not logsource:
			return False

		if logtype=="http":
			retdict = g_httpMatcher.matchALlType(logsource, logrule)
			if retdict:
				logdata.updateDict(retdict)
				self.sendLog(logdata)

	def sendLog(self,logdata):
		level = logdata.GetElement("level")
		logdata.DelElement("level")
		print logdata.getJsonOutput()
		g_logsender.log(logdata.getJsonOutput().decode('utf-8'),1)



	def run(self):
		while True:
			try :
				connection,client_address = self.server_socket.accept()
				threading.Thread(target=self.handler,args=(connection,)).start()
			except KeyboardInterrupt:
				break

	def __del__(self):
		self.server_socket.close()
		os.remove(self.sock_file)





if __name__ == "__main__":
	app = SyslogServer(dlp_socket)
	app.run()