#!/usr/bin/python 
import xml.dom.minidom as xmldom


doc = xmldom.parse("./police.xml")

#keys
root = doc.documentElement


keys = root.getElementsByTagName("key")

for key in keys:
	print("------------------")

	print(key.nodeName)
	#print(key.toxml())
	value = key.childNodes[0].nodeValue
	print value
	print type(value)
