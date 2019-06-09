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
char *replace(char *str, char *srcPtn, char *destPtn, int ignoreCase) // ret: realloc str
{
	autoList<char> *buff = new autoList<char>();

	int srcPtnLen = strlen(srcPtn);
	int destPtnLen = strlen(destPtn);

	errorCase(srcPtnLen < 1);

	for(char *p = str; *p; p++)
	{
		if(!(ignoreCase ? _strnicmp : strncmp)(p, srcPtn, srcPtnLen)) // ? srcPtn ”­Œ©
		{
			buff->AddElements(destPtn, destPtnLen);
			p += srcPtnLen - 1;
		}
		else
			buff->AddElement(*p);
	}
	memFree(str);

	buff->AddElement('\0');
	char *out = buff->UnbindBuffer();
	delete buff;
	return out;
}
char *makeUpper(char *str)
{
	str = strx(str);

	for(char *p = str; *p; mbcInc(p))
		*p = toupper(*p);

	return str;
}

int strcmp_xc(char *a, char *b)
{
	int out = strcmp(a, b);
	memFree(a);
	return out;
}
