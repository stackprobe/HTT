#include "all.h"

char *HRH_Header;
autoList<HeaderField_t *> *HRH_Fields;

int LoadHTTPRequestHeader(char *file) // ret: ? successful
{
	FILE *fp = fileOpen(file, "rb");
	int retval = 0;

	HRH_Header = readLine(fp);

	if(!HRH_Header)
		goto endfunc;

	HRH_Fields = new autoList<HeaderField_t *>();

	for(; ; )
	{
		char *line = readLine(fp);

		if(!line)
			break;

		if(!line[0]) // ? リクエストヘッダ終了
		{
			memFree(line);
			retval = 1;
			break;
		}
		unctrl(line);

		if(line[0] == ' ') // -> unfolding
		{
			if(HRH_Fields->GetCount())
			{
				trim(line);
				HeaderField_t *i = HRH_Fields->GetElement(HRH_Fields->GetCount() - 1);
				i->Value = addChar(i->Value, ' '); // fixme
				i->Value = addLine(i->Value, line);
			}
		}
		else
		{
			char *p = strchr(line, ':');

			if(p)
			{
				*p++ = '\0';
				trim(line);
				trim(p);

				HeaderField_t *i = nb(HeaderField_t);

				i->Name = strx(line);
				i->Value = strx(p);

				HRH_Fields->AddElement(i);
			}
		}
		memFree(line);
	}
endfunc:
	fileClose(fp);
	return retval;
}

int HRH_UrlEndSlash;
autoList<char *> *HRH_UrlPaths;
char *HRH_UserAgent; // HEADER_VALUE_NOT_DEFINED == 無指定
char *HRH_HostName; // HEADER_VALUE_NOT_DEFINED == 無指定
char *HRH_HostPortPart; // NULL == 無指定

static char *DecodeUrl(char *url)
{
	autoList<char> *buff = new autoList<char>();

	for(char *p = url; *p; p++)
	{
		if(*p == '%' && p[1] && p[2])
		{
			buff->AddElement(c2i(p[1]) << 4 | c2i(p[2]));
			p += 2;
		}
		else
			buff->AddElement(*p);
	}
	buff->AddElement('\0');
	char *out = buff->UnbindBuffer();
	delete buff;
	return out;
}
int ParseHTTPRequestHeader(void) // ret: ? successful
{
	char *p = strchr(HRH_Header, ' ');
	if(!p)
		return 0;

	p++;
	char *q = strchr(p, ' ');
	if(!q)
		return 0;

	*q = '\0';
	char *url = DecodeUrl(p);
	url = UTF8ToSJIS_x(url);

	p = strchr(url, '?');
	if(p)
	{
		cout("Remove query!\n");
		*p = '\0';
	}

	p = strstr(url, "://");
	if(p)
	{
		p = strchr(p + 3, '/');
		if(p)
		{
			cout("Remove scheme and domain!\n");
			copyLine(url, p);
		}
	}

	HRH_UrlEndSlash = *getEnd(url, 1) == '/';
	HRH_UrlPaths = tokenize_xc(url, "/");

	for(int index = 0; index < HRH_UrlPaths->GetCount(); index++) // trim ""
	{
		if(HRH_UrlPaths->GetElement(index)[0] == '\0')
		{
			memFree(HRH_UrlPaths->GetElement(index));
			HRH_UrlPaths->DesertElement(index);
			index--;
		}
	}
	char *host = NULL;

	for(int index = 0; index < HRH_Fields->GetCount(); index++)
	{
		HeaderField_t *i = HRH_Fields->GetElement(index);

		if(!_stricmp(i->Name, "Host"))
		{
			host = i->Value;
		}
		else if(!_stricmp(i->Name, "User-Agent"))
		{
			HRH_UserAgent = i->Value;
		}
	}
	if(!HRH_UserAgent)
		HRH_UserAgent = HEADER_VALUE_NOT_DEFINED;

	HRH_UserAgent = strx(HRH_UserAgent); // 念のため複製

	if(host)
	{
		host = strx(host);
		p = strchr(host, ':');
		if(p)
		{
			HRH_HostPortPart = strx(p);
			*p = '\0'; // ポート番号を除去
		}
	}
	else
		host = strx(HEADER_VALUE_NOT_DEFINED);

	HRH_HostName = host;
	return 1;
}
