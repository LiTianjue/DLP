#!/usr/bin/python
import xml.dom.minidom as xmldom

class XmlHelper:
    def __init__(self):
        pass
    def getData(self):
        doc = xmldom.Document()

        root = doc.createElement("log")

        #root.setAttribute("protocol","http")

        #root.setAttribute("type","html")

        doc.appendChild(root)

        nodeproto = doc.createElement("protocol")
        nodetype = doc.createElement("type")
        nodesrc = doc.createElement("src")
        nodedst = doc.createElement("dst")
        nodekey = doc.createElement("key")

        nodeproto.appendChild(doc.createTextNode("http"))
        nodetype.appendChild(doc.createTextNode("html"))
        nodesrc.appendChild(doc.createTextNode("192.168.1.100 1000"))
        nodedst.appendChild(doc.createTextNode("222.46.20.174 12080"))
        nodekey.appendChild(doc.createTextNode("SSL_VPN"))

        root.appendChild(nodeproto)
        root.appendChild(nodetype)
        root.appendChild(nodesrc)
        root.appendChild(nodedst)
        root.appendChild(nodekey)


        fp = open("test.xml",'w')

        doc.writexml(fp,indent='\t',addindent='\t',newl='\n',encoding='utf-8')

        data = doc.toprettyxml(indent='\t',newl='\n',encoding='utf-8')
        #print data
        print type(data)

        fp.close()
        return data

xml_t = XmlHelper()
print xml_t.getData()
