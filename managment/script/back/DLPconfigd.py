#!/usr/bin/python 
#-*- coding:utf-8 -*-
import xml.dom.minidom as xmldom
import pika


#开启RabbitMQ接收服务
#接收到XML格式的关键字信息提取其中的关键字
#发送关键字到 /tmp/dlp.config
config_socket = "/tmp/dlp.config"

def handle_xml(xmlstring):
	doc=xmldom.parseString(xmlstring)
	key_words_list=[]
	#key_words_list.append()
	root = doc.documentElement
	keys = root.getElementsByTagName("key")
	for key in keys:
		key_words_list.append(key)

	print key_words_list

def say_hello(xstring):
	print "hello"

def callback(ch,method,properties,body):
    print "[body type] --> ",type(body)
    print "[x] Received " ,body
	say_hello("2")

connection = pika.BlockingConnection(pika.ConnectionParameters(
        host='45.76.157.192',virtual_host="/", credentials=pika.PlainCredentials("asdf", "123456")))

channel = connection.channel()

channel.queue_declare(queue='IpPortpolice')

channel.basic_consume(callback,
                    queue='IpPortpolice',
                    no_ack=True)

print "[x] Waiting for messages !"
say_hello("1")
channel.start_consuming()

connection.close()
