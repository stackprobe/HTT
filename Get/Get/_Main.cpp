/*
	Get.exe [/H] DOC-ROOT-FILE
*/

#include "all.h"

static void Disconnect(void)
{
	cout("[GET]DISCONNECT @ %s\n", getTimeStamp());

	HTT_Disconnect();
	termination(1);
}
static int GetDownloadIdleTimeoutSec(void) // ret: 下り無通信タイムアウト_秒
{
	int ret = 300;

	switch(TightFlag)
	{
	case 'L': ret = 90; break;
	case 'M': ret = 60; break;
	case 'H': ret = 30; break;
	}
	return ret;
}
static int Download(void) // ? 処理継続
{
	if(1 <= getFileSize(SEND_FILE))
	{
		if(getFileWriteTime(SEND_FILE) + GetDownloadIdleTimeoutSec() < time(NULL)) // ? timeout
			Disconnect();

		return 0;
	}
	FILE *fp = fileOpen(DOWNLOAD_FILE, "rt");
	char *dlFile = readLine(fp);
	__int64 rPos = s2i64_x(readLine(fp));
	fileClose(fp);

	fp = fileOpen(dlFile, "rb");
	__int64 fSize = getFileSize(fp);
	errorCase(rPos < 0 || fSize < rPos);

	if(fSize <= rPos) // ? 送信完了
	{
		if(!existFile(KEEP_ALIVE_FILE))
			Disconnect();

		cout("Keep-Alive Continue to NEXT REQUEST !!!\n");

		removeFile(DUMMY_FILE); // 無いかもしれない。
		removeFile(DOWNLOAD_FILE);
		removeFile(KEEP_ALIVE_FILE);
		return 1;
	}
	int rwSize = (int)m_min(fSize - rPos, 2000000);
	void *buffer = memAlloc(rwSize);

	fileSeek(fp, SEEK_SET, rPos);
	fileRead(fp, buffer, rwSize);
	writeBlock(SEND_FILE, buffer, rwSize, 1);

	memFree(buffer);
	fileClose(fp);

	fp = fileOpen(DOWNLOAD_FILE, "wt");
	writeLine(fp, dlFile);
	writeLine_x(fp, xcout("%I64d", rPos + rwSize));
	fileClose(fp);

	return 0;
}

#define SOFT404HTML_FILE "NotFound.htm"
#define SOFT503HTML_FILE "ServiceUnavailable.htm"

static char *Soft404HtmlFile;
static char *Soft503HtmlFile;

static char *BeforeDLProg;

int main(int argc, char **argv)
{
#if 0 // test
	Debug_GenerateMimeTypeFile();
#endif

	errorCase(!existFile(IP_FILE));
	errorCase(!existFile(RECV_FILE));
	errorCase(!existFile(SEND_FILE));

	if(getFileSize(IP_FILE) == 0) // ? 定期的な空実行
		termination(0);

	TightFlag = 0;

	if(existFile(TIGHT_FILE))
	{
		FILE *fp = fileOpen(TIGHT_FILE, "rb");
		TightFlag = readChar(fp); // "LMH"
		fileClose(fp);
	}
	SendFileFullPath = getFullPath(SEND_FILE);
	ClientIP = readLine(IP_FILE);

	if(existFile(DOWNLOAD_FILE))
	{
		if(!Download())
		{
#if 1
			__int64 recvFileSize = getFileSize(RECV_FILE);

			cout("Donwload_recvFileSize: %I64d\n", recvFileSize);

			// HTTPパイプラインに対応してみる。
			if(REQUEST_BUFFER_SIZE_MAX < recvFileSize)
				Disconnect();
#else
			// HTTP/1.1 の場合、クライアントはレスポンス受信中に次のリクエストを送ってはならない？ ---> HTTPパイプラインというものがあるらしい。
			setFileSize(RECV_FILE, 0); // レスポンス送信中は次のリクエストを受け付けない。
#endif
			termination(0);
		}
//Sleep(2000); // test
//cout("current-time: %I64d\n", time(NULL)); // test
//cout("write-time.1: %I64d\n", getFileWriteTime(RECV_FILE)); // test
		TouchFile(RECV_FILE);
//cout("write-time.2: %I64d\n", getFileWriteTime(RECV_FILE)); // test
	}
	else
	{
		if(getFileWriteTime(RECV_FILE) + 60 < time(NULL)) // ? timeout
			Disconnect();

		if(REQUEST_BUFFER_SIZE_MAX < getFileSize(RECV_FILE))
			Disconnect();
	}
	if(!LoadHTTPRequestHeader(RECV_FILE)) // ? 失敗 || 受信未完了
		termination(0);

	// show header zantei
	{
		char *ip = readLine(IP_FILE);
		char *hdrLine = strx(HRH_Header);

		toAsciiToken(hdrLine);

		cout("%s ==> [%s]\n", ip, hdrLine);

		memFree(ip);
		memFree(hdrLine);

		for(int index = 0; index < HRH_Fields->GetCount(); index++)
		{
			HeaderField_t *i = HRH_Fields->GetElement(index);

			char *hdrName = strx(i->Name);
			char *hdrValue = strx(i->Value);

			toAsciiToken(hdrName);
			toAsciiToken(hdrValue);

			cout("[%s]: [%s]\n", hdrName, hdrValue);

			memFree(hdrName);
			memFree(hdrValue);
		}
	}

	if(!ParseHTTPRequestHeader()) // ? 失敗
		Disconnect();

	WriteLog_Disabled = argIs("/-L");

	WaitLogMutex();

	// Log
	{
//		WriteLog_x(xcout("[%s]", c_makeJStamp(getTimeStamp())));
		WriteLog(c_makeJStamp(getTimeStamp()));
		WriteLog(ClientIP);
		WriteLog_x(xcout("R%s", HRH_Header));
		WriteLog_x(xcout("H%s", HRH_HostName));
		WriteLog_x(xcout("A%s", HRH_UserAgent));
	}

	if(
		HRH_Header[0] != 'G' &&
		HRH_Header[0] != 'H'
		)
	{
		cout("invalid-method\n");
		Disconnect();
	}
	if(!strcmp(HRH_HostName, HEADER_VALUE_NOT_DEFINED))
	{
		cout("no-host\n");
		Disconnect();
	}
	if(argIs("/E:01"))
	{
		LOGPOS();
		char *e01Prog = getFullPath("PutFile.exe", getSelfDir());
		char *commandLine = xcout("\"%s\" /E:02", e01Prog);
		cout("$ %s\n", commandLine);
		system(commandLine);
		memFree(commandLine);
		memFree(e01Prog);
		LOGPOS();
	}
	if(argIs("/P"))
		BeforeDLProg = nextArg();

	int headerOnlyMode = argIs("/H");

	addCwd(getSelfDir());
	LoadDocRootFile(nextArg());
	unaddCwd();

	DocRoot_t *dr = GetDocRoot(HRH_HostName);

	if(!dr)
	{
		dr = GetDocRoot(DEFAULT_HOST);

		if(!dr)
			Disconnect();
	}
	char *target = strx(dr->RootDir);

	for(int index = 0; index < HRH_UrlPaths->GetCount(); index++)
	{
		char *localPath = HRH_UrlPaths->GetElement(index);
		localPath = makeFairLocalPath(localPath);
		target = combine_xx(target, localPath);
	}
	if(existDir(target) && HRH_UrlEndSlash)
		target = combine_xc(target, "index.htm");

	if(!_stricmp(getEnd(target, 4), ".htm") && !existFile(target))
		target = addChar(target, 'l');

	cout("%s %s %s @ %s\n", ClientIP, headerOnlyMode ? "HEAD" : "GET", target, getTimeStamp());

	// Log
	{
		WriteLog_x(xcout("D%s", dr->Domain));
		WriteLog_x(xcout("P%s", target));
	}

	int noContent = 1;
	char *httpStatus = "none";

	cout("TightFlag: %02x\n", TightFlag);

	Soft404HtmlFile = combine(getSelfDir(), SOFT404HTML_FILE);
	Soft503HtmlFile = combine(getSelfDir(), SOFT503HTML_FILE);

	if(TightFlag)
	{
		int htmlFlag = IsMimeHtml(target);

		cout("htmlFlag_503: %d\n", htmlFlag);

		int res503Flag = 0;

		if(htmlFlag)
			res503Flag = TightFlag == 'M' || TightFlag == 'H';
		else
			res503Flag = TightFlag == 'H';

		cout("res503Flag: %d\n", res503Flag);

		if(res503Flag)
		{
			if(htmlFlag && existFile(Soft503HtmlFile))
			{
				LOGPOS();

				FILE *fp = fileOpen(SEND_FILE, "wt");
				writeLine(fp, "HTTP/1.1 200 Server Busy");
				writeLine(fp, "Server: htt");
				writeLine(fp, "Content-Type: text/html");
				writeLine_x(fp, xcout("Content-Length: %I64d", getFileSize(Soft503HtmlFile)));
				writeLine(fp, "Connection: close");
				writeLine(fp, "");
				fileClose(fp);

				strz(target, Soft503HtmlFile);
				noContent = 0;
				httpStatus = "Soft503";
			}
			else
			{
				LOGPOS();

				FILE *fp = fileOpen(SEND_FILE, "wt");
				writeLine(fp, "HTTP/1.1 503 Server Busy");
				writeLine(fp, "Server: htt");
				writeLine(fp, "Connection: close");
				writeLine(fp, "");
				fileClose(fp);

				httpStatus = "503";
			}
			goto endContent;
		}
	}

	int keepAlive = 0;

	if(existFile(target))
	{
		char *targetContentType = "";

		if(BeforeDLProg)
		{
			LOGPOS();
			writeLine(TARGET_FILE, target);
			createFile(TARGET_CONTENT_TYPE_FILE);

			LOGPOS();
			char *commandLine = xcout("\"%s\"", BeforeDLProg);
			cout("$ %s\n", commandLine);
			system(commandLine);
			memFree(commandLine);
			LOGPOS();

			memFree(target);
			target = readLine(TARGET_FILE);
			cout("target_new: %s\n", target);
			targetContentType = readLine(TARGET_CONTENT_TYPE_FILE); // g
			cout("targetContentType_new: [%s]\n", targetContentType);

			removeFile(TARGET_FILE);
			removeFile(TARGET_CONTENT_TYPE_FILE);
		}

		if(*targetContentType == '\0')
			targetContentType = GetMimeType(target);

		if(HRH_KeepAlive)
		{
			cout("Keep-Alive Requested\n");

			if(getFileWriteTime(IP_FILE) + 20 < time(NULL))
			{
				cout("Keep-Alive timeout\n");
			}
			else if(headerOnlyMode) // zantei zantei zantei zantei zantei
			{
				cout("Keep-Alive HEAD\n");
			}
			else
			{
				cout("Keep-Alive OK!\n");
				keepAlive = 1;
			}
			cout("keepAlive: %d\n", keepAlive);
		}
		FILE *fp = fileOpen(SEND_FILE, "wt");
		writeLine(fp, "HTTP/1.1 200 Happy Tea Time");
		writeLine(fp, "Server: htt");
		writeLine_x(fp, xcout("Content-Type: %s", targetContentType));
		writeLine_x(fp, xcout("Content-Length: %I64d", getFileSize(target)));
		writeLine_x(fp, xcout("Connection: %s", keepAlive ? "Keep-Alive" : "close"));
		writeLine(fp, "");
		fileClose(fp);

		noContent = headerOnlyMode;
		httpStatus = "200";
	}
	else if(existDir(target) && !HRH_UrlEndSlash)
	{
		char *url = untokenize(HRH_UrlPaths, "/");

		/*
			urlのエンコードは面倒なので、ASCII限定
		*/
		if(!*url || !isAsciiToken(url))
		{
			memFree(url);
			goto targetNotFound;
		}
		writeLine_cx(SEND_FILE, xcout(
			"HTTP/1.1 301 Hot Chocolate\n"
			"Server: htt\n"
			"Location: http://%s%s/%s/\n"
			"Connection: close\n"
			""
			,HRH_HostName
			,HRH_HostPortPart ? HRH_HostPortPart : ""
			,url
			));
		memFree(url);

		httpStatus = "301";
	}
	else
	{
targetNotFound:
		int htmlFlag = IsMimeHtml(target);

		cout("htmlFlag_404: %d\n", htmlFlag);

		if(htmlFlag && existFile(Soft404HtmlFile))
		{
			FILE *fp = fileOpen(SEND_FILE, "wt");
			writeLine(fp, "HTTP/1.1 200 Coffee Time");
			writeLine(fp, "Server: htt");
			writeLine(fp, "Content-Type: text/html");
			writeLine_x(fp, xcout("Content-Length: %I64d", getFileSize(Soft404HtmlFile)));
			writeLine(fp, "Connection: close");
			writeLine(fp, "");
			fileClose(fp);

			strz(target, Soft404HtmlFile);
			noContent = 0;
			httpStatus = "Soft404";
		}
		else
		{
			writeLine(SEND_FILE,
				"HTTP/1.1 404 Coffee Time\n"
				"Server: htt\n"
				"Connection: close\n"
				""
				);

			httpStatus = "404";
		}
	}

endContent:
	if(noContent)
	{
		createFile(DUMMY_FILE);
		strz(target, DUMMY_FILE);
	}

	// Log
	{
		WriteLog_x(xcout("S%s", httpStatus));
	}

	{
		FILE *fp = fileOpen(DOWNLOAD_FILE, "wt");
		writeLine(fp, target);
		writeLine(fp, "0");
		fileClose(fp);
	}

	if(keepAlive)
		createFile(KEEP_ALIVE_FILE);

//	setFileSize(RECV_FILE, 0); // --> DeleteFileDataPart_BeforeFP() @ LoadHTTPRequestHeader()

	memFree(target);

	termination(0);
}
