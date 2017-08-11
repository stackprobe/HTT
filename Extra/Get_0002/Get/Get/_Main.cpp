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
static void Download(void)
{
	setFileSize(RECV_FILE, 0);

	if(1 <= getFileSize(SEND_FILE))
	{
		if(getFileWriteTime(SEND_FILE) + 300 < time(NULL)) // ? timeout
			Disconnect();

		return;
	}
	FILE *fp = fileOpen(DOWNLOAD_FILE, "rt");
	char *dlFile = readLine(fp);
	__int64 rPos = s2i64_x(readLine(fp));
	fileClose(fp);

	fp = fileOpen(dlFile, "rb");
	__int64 fSize = getFileSize(fp);
	errorCase(rPos < 0 || fSize < rPos);

	if(fSize <= rPos) // ? 送信完了
		Disconnect();

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
}

#define SOFT404HTML_FILE "NotFound.htm"
#define SOFT503HTML_FILE "ServiceUnavailable.htm"

static char *Soft404HtmlFile;
static char *Soft503HtmlFile;

int main(int argc, char **argv)
{
	errorCase(!existFile(IP_FILE));
	errorCase(!existFile(RECV_FILE));
	errorCase(!existFile(SEND_FILE));

	if(getFileSize(IP_FILE) == 0) // ? 定期的な空実行
		termination(0);

	int tightFlag = 0; // 0 or "LMH"

	if(existFile(TIGHT_FILE))
	{
		FILE *fp = fileOpen(TIGHT_FILE, "rb");
		tightFlag = readChar(fp);
		fileClose(fp);
	}
//cout("tightFlag: %d\n", tightFlag); // test

	SendFileFullPath = getFullPath(SEND_FILE);
	ClientIP = readLine(IP_FILE);

	if(existFile(DOWNLOAD_FILE))
	{
		Download();
		termination(0);
	}
	if(getFileWriteTime(RECV_FILE) + 60 < time(NULL)) // ? timeout
		Disconnect();

	if(512000 < getFileSize(RECV_FILE)) // ? リクエストヘッダが大きすぎる。
		Disconnect();

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

	WaitLogMutex();

	// Log
	{
		WriteLog_x(xcout(
			"%s %s \"%s\" Host:\"%s\" User-Agent:\"%s\""
			,c_makeJStamp(getTimeStamp())
			,ClientIP
			,HRH_Header
			,HRH_HostName
			,HRH_UserAgent
			));
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
		WriteLog_x(xcout(
			"Domain:\"%s\" Target-Path:\"%s\""
			,dr->Domain
			,target
			));
	}

	int noContent = 1;
	char *httpStatus = "none";

	cout("tightFlag: %02x\n", tightFlag);

	Soft404HtmlFile = combine(getSelfDir(), SOFT404HTML_FILE);
	Soft503HtmlFile = combine(getSelfDir(), SOFT503HTML_FILE);

	if(tightFlag)
	{
		int htmlFlag = IsMimeHtml(target);

		cout("htmlFlag_503: %d\n", htmlFlag);

		int res503Flag = 0;

		if(htmlFlag)
			res503Flag = tightFlag == 'M' || tightFlag == 'H';
		else
			res503Flag = tightFlag == 'H';

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

	if(existFile(target))
	{
		FILE *fp = fileOpen(SEND_FILE, "wt");
		writeLine(fp, "HTTP/1.1 200 Happy Tea Time");
		writeLine(fp, "Server: htt");
		writeLine_x(fp, xcout("Content-Type: %s", GetMimeType(target)));
		writeLine_x(fp, xcout("Content-Length: %I64d", getFileSize(target)));
		writeLine(fp, "Connection: close");
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
		WriteLog_x(xcout(
			"Status: %s"
			,httpStatus
			));
	}

	{
		FILE *fp = fileOpen(DOWNLOAD_FILE, "wt");
		writeLine(fp, target);
		writeLine(fp, "0");
		fileClose(fp);
	}
	memFree(target);

	termination(0);
}
