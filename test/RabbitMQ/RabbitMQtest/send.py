#!/usr/bin/python
import pika
import creatXML


connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost',credentials=pika.PlainCredentials("admin","admin123")))

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
                    routing_key='hello',
                    body=data)

print "[x] Sent 'hello world' !"

connection.close()
