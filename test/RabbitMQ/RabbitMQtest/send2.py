#!/usr/bin/python
import pika
import creatXML


#connection = pika.BlockingConnection(pika.ConnectionParameters(host="localhost", virtual_host="/", credentials=pika.PlainCredentials("guest", "guest")))

connection = pika.BlockingConnection(pika.ConnectionParameters(host="45.76.157.192", virtual_host="/", credentials=pika.PlainCredentials("asdf", "123456")))

channel = connection.channel()

channel.queue_declare(queue='IpPortpolice')

xml_t = creatXML.XmlHelper()
data = xml_t.getData()

'''
channel.basic_publish(exchange='',
                    routing_key='hello',
                    body="Hello World!")
'''
channel.basic_publish(exchange='',
                    routing_key='IpPortpolice',
                    body=data)

print "[x] Sent 'hello world' !"

connection.close()
