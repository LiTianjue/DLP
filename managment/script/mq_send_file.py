#!/usr/bin/python
import pika
import sys

fd = open(sys.argv[1])
data = fd.read()
fd.close()


#connection = pika.BlockingConnection(pika.ConnectionParameters(host="localhost", virtual_host="/", credentials=pika.PlainCredentials("guest", "guest")))

connection = pika.BlockingConnection(pika.ConnectionParameters(host="45.32.102.24", virtual_host="/", credentials=pika.PlainCredentials("asdf", "123456")))

channel = connection.channel()

channel.queue_declare(queue='sensitive_word')

#xml_t = creatXML.XmlHelper()
#data = xml_t.getData()

'''
channel.basic_publish(exchange='',
                    routing_key='hello',
                    body="Hello World!")
'''
channel.basic_publish(exchange='',
                    routing_key='sensitive_word',
                    body=data)

print "[x] Sent 'hello world' !"

connection.close()
