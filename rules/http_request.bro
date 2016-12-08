
#http://222.46.20.174:12080/UpgradeVersionAction_check.action?os=x86&version=2.4.0.0

@load base/protocols/http
@load base/bif

#redef capture_filters += {
#	["http-request"] = "tcp port 12080"
#};


global mime_to_ext: table[string] of string = {
        ["application/x-dosexec"] = "exe",
        ["text/plain"] = "txt",
        ["image/jpeg"] = "jpg",
        ["image/png"] = "png",
        ["text/html"] = "html",
        ["text/json"] = "json",
		["text/xml"] = "xml",
		["application/xml"] = "xml",
};

function formatMsg(protocol:string,rule:string,src:string,dst:string,uri:string,source:string):string
	{
	local json:string = fmt("{\\\"protocol\\\" : \\\"%s\\\", \\\"rule\\\": \\\"%s\\\" ,\\\"src\\\":\\\"%s\\\",\\\"dst\\\" : \\\"%s\\\",\\\"uri\\\":\\\"%s\\\",\\\"source\\\":\\\"%s\\\"   }",protocol,rule,src,dst,uri,source);
	return json;
	}

function sendlog(msg:string)
	{
		local cmdline = fmt("echo \" %s \" | socat stdin /tmp/dlp.sock &",msg);
		system(cmdline);
	}



const sensitive_URIs = 
		/\/cgi-bin /
		| /\/cmd/
		&redef;

event http_request(c: connection,method:string, original_URL:string,
	unescaped_URI:string,version:string)
	{
		#print "---------------request-------------";
#print "http request",c$id,original_URL;
#print "http_request event-----"+"method="+method,"original_URL="+original_URL,"version:"+version;
		#system(fmt("./url_check.sh \" %s \"  \" %s \"  ",original_URL,c$id));
		#print "----------------------------";
		
		local src:string =fmt("%s %d",c$id$orig_h,port_to_count(c$id$orig_p));
		local dst:string =fmt("%s %d",c$id$resp_h,port_to_count(c$id$resp_p));

		#print src;
		#print dst;
		local msg:string = formatMsg("http","uri",src,dst,original_URL,original_URL);
		sendlog(msg);

#print "from:",c$id$orig_h,port_to_count(c$id$orig_p),"to:",c$id$resp_h,port_to_count(c$id$resp_p);
		#print "URI:",original_URL;
	
	}

event http_reply(c: connection, version: string, code: count, reason: string) 
        {
    		print fmt("---------------reply[%d:%s]-------------",code,reason);
    		#print c$id;
    		#print c$conn;
        	#print "----------------------------";
        }

event file_sniff(f: fa_file, meta: fa_metadata)
	{
		#print "file sniff",f$source;
		#print meta;
		if( f$source != "HTTP")
			return;

		#print f$http;
		#print f;

#		if(! meta?$mime_type)
#			return;
		if( meta?$mime_type)
			print meta$mime_type;
		

		#print f$http;

		
#if ( meta$mime_type !in mime_to_ext )
#               return;

		local fname:string;
		if ( !meta?$mime_type || meta$mime_type !in mime_to_ext)
		{
			fname = fmt("%s-%s.bin", f$source, f$id);
		}
		else {
			fname = fmt("%s-%s.%s", f$source, f$id, mime_to_ext[meta$mime_type]);
		}

		local ruletype:string = mime_to_ext[meta$mime_type];
		print ruletype;
		if((ruletype=="html") || (ruletype=="xml"))
			{
				print fmt("Extracting file %s", fname);
				Files::add_analyzer(f, Files::ANALYZER_EXTRACT, [$extract_filename=fname]);
				local src:string = fmt("%s %d",f$http$id$orig_h,port_to_count(f$http$id$orig_p));
				local dst:string = fmt("%s %d",f$http$id$resp_h,port_to_count(f$http$id$resp_p));
				local msg:string = formatMsg("http",ruletype,src,dst,f$http$uri,fname);
				sendlog(msg);
			}
	}
