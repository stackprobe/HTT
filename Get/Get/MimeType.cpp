#include "all.h"

char *DefaultMimeType = "application/octet-stream";

char *MimeList[][2] =
{
	"323", "text/h323",
	"acx", "application/internet-property-stream",
	"ai", "application/postscript",
	"aif", "audio/x-aiff",
	"aifc", "audio/x-aiff",
	"aiff", "audio/x-aiff",
	"asf", "video/x-ms-asf",
	"asr", "video/x-ms-asf",
	"asx", "video/x-ms-asf",
	"au", "audio/basic",
	"avi", "video/x-msvideo",
	"axs", "application/olescript",
	"bas", "text/plain",
	"bcpio", "application/x-bcpio",
	"bin", "application/octet-stream",
	"bmp", "image/bmp",
	"c", "text/plain",
	"cat", "application/vnd.ms-pkiseccat",
	"cdf", "application/x-cdf",
	"cer", "application/x-x509-ca-cert",
	"class", "application/octet-stream",
	"clp", "application/x-msclip",
	"cmx", "image/x-cmx",
	"cod", "image/cis-cod",
	"cpio", "application/x-cpio",
	"crd", "application/x-mscardfile",
	"crl", "application/pkix-crl",
	"crt", "application/x-x509-ca-cert",
	"csh", "application/x-csh",
	"css", "text/css",
	"dcr", "application/x-director",
	"der", "application/x-x509-ca-cert",
	"dir", "application/x-director",
	"dll", "application/x-msdownload",
	"dms", "application/octet-stream",
	"doc", "application/msword",
	"dot", "application/msword",
	"dvi", "application/x-dvi",
	"dxr", "application/x-director",
	"eps", "application/postscript",
	"etx", "text/x-setext",
	"evy", "application/envoy",
	"exe", "application/octet-stream",
	"fif", "application/fractals",
	"flr", "x-world/x-vrml",
	"gif", "image/gif",
	"gtar", "application/x-gtar",
	"gz", "application/x-gzip",
	"h", "text/plain",
	"hdf", "application/x-hdf",
	"hlp", "application/winhlp",
	"hqx", "application/mac-binhex40",
	"hta", "application/hta",
	"htc", "text/x-component",
	"htm", "text/html",
	"html", "text/html",
	"htt", "text/webviewhtml",
	"ico", "image/x-icon",
	"ief", "image/ief",
	"iii", "application/x-iphone",
	"ins", "application/x-internet-signup",
	"isp", "application/x-internet-signup",
	"jfif", "image/pipeg",
	"jpe", "image/jpeg",
	"jpeg", "image/jpeg",
	"jpg", "image/jpeg",
	"js", "application/x-javascript",
	"latex", "application/x-latex",
	"lha", "application/octet-stream",
	"lsf", "video/x-la-asf",
	"lsx", "video/x-la-asf",
	"lzh", "application/octet-stream",
	"m13", "application/x-msmediaview",
	"m14", "application/x-msmediaview",
	"m3u", "audio/x-mpegurl",
	"man", "application/x-troff-man",
	"mdb", "application/x-msaccess",
	"me", "application/x-troff-me",
	"mht", "message/rfc822",
	"mhtml", "message/rfc822",
	"mid", "audio/mid",
	"mny", "application/x-msmoney",
	"mov", "video/quicktime",
	"movie", "video/x-sgi-movie",
	"mp2", "video/mpeg",
	"mp3", "audio/mpeg",
	"mpa", "video/mpeg",
	"mpe", "video/mpeg",
	"mpeg", "video/mpeg",
	"mpg", "video/mpeg",
	"mpp", "application/vnd.ms-project",
	"mpv2", "video/mpeg",
	"ms", "application/x-troff-ms",
	"mvb", "application/x-msmediaview",
	"nws", "message/rfc822",
	"oda", "application/oda",
	"p10", "application/pkcs10",
	"p12", "application/x-pkcs12",
	"p7b", "application/x-pkcs7-certificates",
	"p7c", "application/x-pkcs7-mime",
	"p7m", "application/x-pkcs7-mime",
	"p7r", "application/x-pkcs7-certreqresp",
	"p7s", "application/x-pkcs7-signature",
	"pbm", "image/x-portable-bitmap",
	"pdf", "application/pdf",
	"pfx", "application/x-pkcs12",
	"pgm", "image/x-portable-graymap",
	"pko", "application/ynd.ms-pkipko",
	"pma", "application/x-perfmon",
	"pmc", "application/x-perfmon",
	"pml", "application/x-perfmon",
	"pmr", "application/x-perfmon",
	"pmw", "application/x-perfmon",
	"pnm", "image/x-portable-anymap",
	"pot", "application/vnd.ms-powerpoint",
	"ppm", "image/x-portable-pixmap",
	"pps", "application/vnd.ms-powerpoint",
	"ppt", "application/vnd.ms-powerpoint",
	"prf", "application/pics-rules",
	"ps", "application/postscript",
	"pub", "application/x-mspublisher",
	"qt", "video/quicktime",
	"ra", "audio/x-pn-realaudio",
	"ram", "audio/x-pn-realaudio",
	"ras", "image/x-cmu-raster",
	"rgb", "image/x-rgb",
	"rmi", "audio/mid",
	"roff", "application/x-troff",
	"rtf", "application/rtf",
	"rtx", "text/richtext",
	"scd", "application/x-msschedule",
	"sct", "text/scriptlet",
	"setpay", "application/set-payment-initiation",
	"setreg", "application/set-registration-initiation",
	"sh", "application/x-sh",
	"shar", "application/x-shar",
	"sit", "application/x-stuffit",
	"snd", "audio/basic",
	"spc", "application/x-pkcs7-certificates",
	"spl", "application/futuresplash",
	"src", "application/x-wais-source",
	"sst", "application/vnd.ms-pkicertstore",
	"stl", "application/vnd.ms-pkistl",
	"stm", "text/html",
	"sv4cpio", "application/x-sv4cpio",
	"sv4crc", "application/x-sv4crc",
	"t", "application/x-troff",
	"tar", "application/x-tar",
	"tcl", "application/x-tcl",
	"tex", "application/x-tex",
	"texi", "application/x-texinfo",
	"texinfo", "application/x-texinfo",
	"tgz", "application/x-compressed",
	"tif", "image/tiff",
	"tiff", "image/tiff",
	"tr", "application/x-troff",
	"trm", "application/x-msterminal",
	"tsv", "text/tab-separated-values",
	"txt", "text/plain",
	"uls", "text/iuls",
	"ustar", "application/x-ustar",
	"vcf", "text/x-vcard",
	"vrml", "x-world/x-vrml",
	"wav", "audio/x-wav",
	"wcm", "application/vnd.ms-works",
	"wdb", "application/vnd.ms-works",
	"wks", "application/vnd.ms-works",
	"wmf", "application/x-msmetafile",
	"wps", "application/vnd.ms-works",
	"wri", "application/x-mswrite",
	"wrl", "x-world/x-vrml",
	"wrz", "x-world/x-vrml",
	"xaf", "x-world/x-vrml",
	"xbm", "image/x-xbitmap",
	"xla", "application/vnd.ms-excel",
	"xlc", "application/vnd.ms-excel",
	"xlm", "application/vnd.ms-excel",
	"xls", "application/vnd.ms-excel",
	"xlt", "application/vnd.ms-excel",
	"xlw", "application/vnd.ms-excel",
	"xof", "x-world/x-vrml",
	"xpm", "image/x-xpixmap",
	"xwd", "image/x-xwindowdump",
	"z", "application/x-compress",
	"zip", "application/zip",

	// 2014.9.7 í«â¡

	"txt",  "text/plain",
	"htm",  "text/html",
	"html", "text/html",

	"xml",  "text/xml",
	"xsl",  "text/xml",

	"bmp",  "image/bmp",
	"gif",  "image/gif",
	"jpg",  "image/jpeg",
	"jpeg", "image/jpeg",
	"png",  "image/png",
	"tif",  "image/tiff",
	"tiff", "image/tiff",
	"ico",  "image/vnd.microsoft.icon",

	"mid",  "audio/midi",
	"midi", "audio/midi",
	"mp2",  "audio/mpeg",
	"mp3",  "audio/mpeg",
	"mpga", "audio/mpeg",

	"mpe",  "video/mpeg",
	"mpeg", "video/mpeg",
	"mpg",  "video/mpeg",

	"pdf",  "application/pdf",
	"zip",  "application/zip",
	"xht",  "application/xhtml+xml",
	"xhtml","application/xhtml+xml",
	"xls",  "application/vnd.ms-excel",
	"doc",  "application/msword",

	// 2016.4.16 í«â¡

	"swf",  "application/x-shockwave-flash",
	"webm", "video/webm",

	// 2016.6.13 í«â¡

	"m4a", "audio/aac",
	"m4v", "video/mp4",
	"mp4", "video/mp4",
	"ogv", "video/ogg",

	// 2018.2.2 í«â¡

	"svg", "image/svg+xml",
};

void Debug_GenerateMimeTypeFile()
{
	FILE *fp = fileOpen("C:\\temp\\" MIME_TYPE_FILE "_", "wt");

	for(int index = 0; index < lengthof(MimeList); index++)
	{
		writeLine_x(fp, xcout("%s\t%s", MimeList[index][0], MimeList[index][1]));
	}
	fileClose(fp);
}

char *GetMimeType(char *file)
{
	char *ext = getExt(file);

	{
		char *mimeTypeFile = combine(getSelfDir(), MIME_TYPE_FILE); // g

		if(existFile(mimeTypeFile))
		{
			autoList<char *> *lines = readLines(mimeTypeFile); // g

			for(int index = 0; index < lines->GetCount(); index++)
			{
				char *line = lines->GetElement(index);
				char *p = strchr(line, '\t');

				errorCase(!p);

				*p = '\0';

				if(!_stricmp(line, ext))
				{
					return p + 1; // íçà”ÅFp ÇÕ lines ÇÃàÍïî
				}
			}
			return DefaultMimeType;
		}
	}

	for(int index = 0; index < lengthof(MimeList); index++)
	{
		if(!_stricmp(ext, MimeList[index][0]))
		{
			return MimeList[index][1];
		}
	}
	return DefaultMimeType;
}
int IsMimeHtml(char *file)
{
	char *ext = getExt(file);

	return
		!_stricmp(ext, "htm") ||
		!_stricmp(ext, "html");
}
