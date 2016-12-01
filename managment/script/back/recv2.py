#!/usr/bin/python
import pika



def callback(ch,method,properties,body):
    print "[body type] --> ",type(body)
    print "[x] Received " ,body
    #print "[x] Received %r" % (body,)

connection = pika.BlockingConnection(pika.ConnectionParameters(
        host='45.76.157.192',virtual_host="/", credentials=pika.PlainCredentials("asdf", "123456")))

channel = connection.channel()

channel.queue_declare(queue='IpPortpolice')

channel.basic_consume(callback,
                    queue='IpPortpolice',
                    no_ack=True)

print "[x] Waiting for messages !"
channel.start_consuming()

connection.close()
