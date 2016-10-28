

function formatMsg(protocol:string,rule:string,src:string,dst:string,uri:string,source:string):string
{
	local json:string = fmt("{\\\"protocol\\\" : \\\"%s\\\", \\\"rule\\\": \\\"%s\\\" ,\\\"src\\\":\\\"%s\\\",\\\"dst\\\" : \\\"%s\\\",\\\"uri\\\":\\\"%s\\\",\\\"source\\\":\\\"%s\\\"   }",protocol,rule,src,dst,uri,source);
	return json;
}



event bro_init()
	{
	print "Hello Bro";


	local json  = formatMsg("http","uri","192.168.1.100 8923","222.46.20.174 12080","SetDefautPasswd","SetDefaultPasswd");
	local cmdline = fmt("echo \" %s  \"  | socat stdin /tmp/dlp.sock &",json);
	system(cmdline);

	
	}

event bro_done()
{
	print "GoodByte Bro";
}
