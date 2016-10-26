
#http://222.46.20.174:12080/UpgradeVersionAction_check.action?os=x86&version=2.4.0.0

@load base/protocols/http

redef capture_filters += {
	["http-request"] = "tcp port 12080 or tcp port 8080 or tcp port 80"
};


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


const sensitive_URIs = 
		/\/cgi-bin /
		| /\/cmd/
		&redef;

event http_request(c: connection,method:string, original_URL:string,
	unescaped_URI:string,version:string)
	{
		#print "---------------request-------------";
		print "http request",c$id,original_URL;
		#print "http_request event-----"+"method="+method,"original_URL="+original_URL,"version:"+version;
		system(fmt("./url_check.sh \" %s \"  \" %s \"  ",original_URL,c$id));
		#print "----------------------------";
	
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

		print fmt("Extracting file %s", fname);
		Files::add_analyzer(f, Files::ANALYZER_EXTRACT, [$extract_filename=fname]);
		system(fmt("./key_match.sh extract_files/%s",fname));

	}
