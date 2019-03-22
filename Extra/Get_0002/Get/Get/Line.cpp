#include "all.h"

char *strx(char *line)
{
	return strcpy((char *)memAlloc(strlen(line) + 1), line);
}
char *strr(char *line)
{
	return (char *)memRealloc(line, strlen(line) + 1);
}
void strz(char *&buffer, char *line)
{
	memFree(buffer);
	buffer = strx(line);
}
void strz_x(char *&buffer, char *line)
{
	memFree(buffer);
	buffer = line;
}

char *getConstNullString(void)
{
	return "";
}
char *getNullString(void)
{
	return strx("");
}

autoList<char *> *tokenize(char *line, char *delimiters)
{
	autoList<char *> *tokens = new autoList<char *>();
	autoList<char> *token = new autoList<char>();

	for(char *p = line; *p; p++)
	{
		char *d;

		for(d = delimiters; *d; d++)
			if(*d == *p)
				break;

		if(*d)
		{
			token->AddElement('\0');
			tokens->AddElement(token->UnbindBuffer());
		}
		else
			token->AddElement(*p);
	}
	token->AddElement('\0');
	tokens->AddElement(token->UnbindBuffer());

	delete token;
	return tokens;
}
autoList<char *> *tokenize_xc(char *line, char *delimiters)
{
	autoList<char *> *out = tokenize(line, delimiters);
	memFree(line);
	return out;
}
char *untokenize(autoList<char *> *tokens, char *separator)
{
	autoList<char> *buffer = new autoList<char>();

	for(int index = 0; index < tokens->GetCount(); index++)
	{
		char *token = tokens->GetElement(index);

		if(index)
			buffer->AddElements(separator, strlen(separator));

		buffer->AddElements(token, strlen(token));
	}
	buffer->AddElement('\0');
	char *line = buffer->UnbindBuffer();
	delete buffer;
	return line;
}
char *untokenize_xc(autoList<char *> *tokens, char *separator)
{
	char *out = untokenize(tokens, separator);
	releaseList(tokens, (void (*)(char *))memFree);
	return out;
}

#define CB_MINSIZE 16

static uchar CB_Buff[CB_MINSIZE];

void copyBlock(void *dest, void *src, int size) // 20140622_debug
{
	if(dest == src)
		return;

	if(size == 0)
		return;

	if((uchar *)dest + size <= src || (uchar *)src + size <= dest) // ? d•¡ƒiƒV
	{
		memcpy(dest, src, size);
	}
	else if(size <= CB_MINSIZE)
	{
		memcpy(CB_Buff, src, size);
		memcpy(dest, CB_Buff, size);
	}
	else
	{
		int half = size / 2;

		if(dest < src)
			copyBlock(dest, src, half);

		copyBlock((uchar *)dest + half, (uchar *)src + half, size - half);

		if(src < dest)
			copyBlock(dest, src, half);
	}
}
void copyLine(char *dest, char *src)
{
	copyBlock(dest, src, strlen(src) + 1);
}

void trimLead(char *line, int chr)
{
	char *p;

	for(p = line; *p; p++)
		if(*p != chr)
			break;

	copyLine(line, p);
}
void trimTrail(char *line, int chr)
{
	char *p;

	for(p = strchr(line, '\0'); line < p; p--)
		if(p[-1] != chr)
			break;

	*p = '\0';
}
void trimEdge(char *line, int chr)
{
	trimLead(line, chr);
	trimTrail(line, chr);
}
void trimSequ(char *line, int delimChr)
{
	/*
		debug segv @ 2016.4.7
	*/

	char *n = line;

	errorCase(delimChr == '\0');

	while(n = strchr(n, delimChr))
	{
		n++;

		if(*n == delimChr)
		{
			char *f = n;

			do
			{
				f++;
			}
			while(*f == delimChr);

			while(*f)
			{
				if(*f == delimChr)
					while(f[1] == delimChr)
						f++;

				*n++ = *f++;
			}
			*n = '\0';
			break;
		}
	}
}
void trim(char *line, int chr)
{
	trimEdge(line, chr);
	trimSequ(line, chr);
}
void unctrl(char *line)
{
	for(char *p = line; *p; p++)
		if(*p < '\x20')
			*p = '\x20';
}

void replaceMbsChar(char *line, int chr1, int chr2)
{
	for(char *p = line; *p; mbcInc(p))
		if(*p == chr1)
			*p = chr2;
}
void escapeYen(char *line)
{
	replaceMbsChar(line, '\\', '/');
}
void unescapeYen(char *line)
{
	replaceMbsChar(line, '/', '\\');
}

char *addToken(char *buffer, char *token)
{
	char *newBuffer = xcout("%s%s", buffer, token);
	memFree(buffer);
	return newBuffer;
}
char *addToken_x(char *buffer, char *token)
{
	char *newBuffer = addToken(buffer, token);
	memFree(token);
	return newBuffer;
}
char *addChar(char *buffer, int chr)
{
	char *newBuffer = xcout("%s%c", buffer, chr);
	memFree(buffer);
	return newBuffer;
}
char *addLine(char *buffer, char *line)
{
	buffer = addToken(buffer, line);
	buffer = addChar(buffer, '\n');
	return buffer;
}
char *addLine_x(char *buffer, char *line)
{
	char *newBuffer = addLine(buffer, line);
	memFree(line);
	return newBuffer;
}

char *getEnd(char *str, int num)
{
	int len = strlen(str);
	return str + len - m_min(num, len);
}
char *getEnd(char *str, char *ptn)
{
	return getEnd(str, strlen(ptn));
}

int c2lc(int chr)
{
	if('A' <= chr && chr <= 'Z') return chr + 0x20;
	return chr;
}
int c2uc(int chr)
{
	if('a' <= chr && chr <= 'z') return chr - 0x20;
	return chr;
}

int my_strnicmp(char *str1, char *str2, int count)
{
	int retval = 0;

	for(int index = 0; index < count; index++)
	{
		if(str1[index] == '\0' && str2[index] == '\0')
			break;

		retval = c2lc(str1[index]) - c2lc(str2[index]);

		if(retval != 0)
			break;
	}
	return retval;
}

int isRangeToken(char *token, int minchr, int maxchr)
{
	for(char *p = token; *p; p++)
		if(m_isRange(*p, minchr, maxchr) == 0)
			return 0;

	return 1;
}
int isAsciiToken(char *token)
{
	return isRangeToken(token, '\x21', '\x7e');
}
void toAsciiToken(char *token)
{
	for(char *p = token; *p; p++)
		if(m_isRange(*p, '\x20', '\x7e') == 0)
			*p = '?';
}
void toJToken(char *token)
{
	for(char *p = token; *p; p++)
	{
		if(isJChar(p))
			p++;
		else if(m_isRange(*p, '\x20', '\x7e') == 0)
			*p = '?';
	}
}
