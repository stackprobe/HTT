#include "all.h"

FILE *rfopen(char *file, char *mode)
{
	for(int retry = 0; ; retry++)
	{
		// fopen() ÇÕà¯êîÇ…ñ‚ëËÇ™ñ≥Ç≠ÇƒÇ‡ÅAãHÇ…é∏îsÇ∑ÇÈÇ±Ç∆Ç™Ç†ÇÈÅB
		// ìKìñÇ…ä‘äuÇãÛÇØÇƒâΩìxÇ©ÉäÉgÉâÉCÇ∑ÇÈÅB
		FILE *fp = fopen(file, mode); // file == "" ÇÃÇ∆Ç´ NULL Çï‘Ç∑ÅB

		if(fp != NULL)
		{
			if(retry)
				cout("File opened at %u-th retrial.\n", retry);

			return fp;
		}
		if(retry == 20)
		{
			cout("Can not open file \"%s\" as \"%s\" mode.\n", file, mode);
			return NULL;
		}

		// sync > @ HTT_rfopen_CheckDisease

		{
			DWORD lastError = GetLastError();

			if(lastError == ERROR_VIRUS_INFECTED || lastError == ERROR_VIRUS_DELETED)
			{
				LOGPOS();
				cout("file: \"%s\"\n", file);
				cout("mode: \"%s\"\n", mode);
				cout("lastError: %d\n", (int)lastError);
				return NULL;
			}
		}

		// < sync

		Sleep(100);
	}
}
FILE *fileOpen(char *file, char *mode)
{
	FILE *fp = rfopen(file, mode);

	errorCase(!fp);
	return fp;
}
void fileClose(FILE *fp)
{
	errorCase(fclose(fp)); // ? é∏îs
}

__int64 getFileSeekPos(FILE *fp)
{
	__int64 pos = _ftelli64(fp);
	errorCase(pos < 0); // ? é∏îs
	return pos;
}
__int64 getFileSize(FILE *fp)
{
	errorCase(_fseeki64(fp, 0I64, SEEK_END)); // ? é∏îs
	return getFileSeekPos(fp);
}
__int64 getFileSize(char *file)
{
	FILE *fp = fileOpen(file, "rb");

	__int64 size = getFileSize(fp);

	fileClose(fp);
	return size;
}
void setFileSize(char *file, __int64 size)
{
	FILE *fp = fileOpen(file, "ab");
	int fh = _fileno(fp);

	errorCase(_chsize_s(fh, size)); // ? é∏îs

	fileClose(fp);
}

size_t getFileSeekPos32(FILE *fp, size_t retMax)
{
	__int64 pos = getFileSeekPos(fp);
	errorCase((__int64)retMax < pos);
	return (size_t)pos;
}
size_t getFileSize32(FILE *fp, size_t retMax)
{
	__int64 size = getFileSize(fp);
	errorCase((__int64)retMax < size);
	return (size_t)size;
}

int readChar(FILE *fp)
{
	int chr = fgetc(fp);

	errorCase(chr == EOF && ferror(fp)); // ? é∏îs
	return chr;
}
char *readLine(FILE *fp)
{
	autoList<char> *buff = new autoList<char>();

	for(; ; )
	{
		int chr = readChar(fp);

		if(chr == '\r')
			continue;

		if(chr == '\n')
			break;

		if(chr == EOF)
		{
			if(!buff->GetCount())
			{
				delete buff;
				return NULL;
			}
			break;
		}
		buff->AddElement(chr);
	}
	buff->AddElement('\0');
	char *line = buff->UnbindBuffer();
	delete buff;
	return line;
}
char *readLine(char *file)
{
	FILE *fp = fileOpen(file, "rt");
	char *line = readLine(fp);
	fileClose(fp);

	if(!line)
		line = strx("");

	return line;
}
autoList<char *> *readLines(char *file)
{
	FILE *fp = fileOpen(file, "rt");
	autoList<char *> *lines = new autoList<char *>();

	for(; ; )
	{
		char *line = readLine(fp);

		if(!line)
			break;

		lines->AddElement(line);
	}
	fileClose(fp);
	return lines;
}

void writeChar(FILE *fp, int chr)
{
	errorCase(fputc(chr, fp) == EOF); // ? é∏îs
}
void writeToken(FILE *fp, char *token)
{
	for(char *p = token; *p; p++)
	{
		writeChar(fp, *p);
	}
}
void writeLine(FILE *fp, char *line)
{
	writeToken(fp, line);
	writeChar(fp, '\n');
}
void writeLine_x(FILE *fp, char *line)
{
	writeLine(fp, line);
	memFree(line);
}
void writeLine(char *file, char *line)
{
	FILE *fp = fileOpen(file, "wt");
	writeLine(fp, line);
	fileClose(fp);
}
void writeLine_cx(char *file, char *line)
{
	writeLine(file, line);
	memFree(line);
}
void writeAddLine(char *file, char *line)
{
	FILE *fp = fileOpen(file, "at");
	writeLine(fp, line);
	fileClose(fp);
}

void fileSeek(FILE *fp, int origin, __int64 offset) // origin, offset ÇÃï¿Ç—ÇÕ fseek() Ç∆ãt
{
	errorCase(_fseeki64(fp, offset, origin)); // ? é∏îs
}
void fileRead(FILE *fp, void *block, int size)
{
	if(size)
	{
		errorCase(fread(block, 1, size, fp) != size); // ? é∏îs || size ÉoÉCÉgì«Ç›çûÇﬁëOÇ… EOF Ç…ìûíB
		errorCase(ferror(fp));
	}
}
void fileWrite(FILE *fp, void *block, int size)
{
	if(size)
	{
		errorCase(fwrite(block, 1, size, fp) != size); // ? é∏îs
		errorCase(ferror(fp));
	}
}

void writeBlock(char *file, void *block, int size, int addMode)
{
	FILE *fp = fileOpen(file, addMode ? "ab" : "wb");
	fileWrite(fp, block ,size);
	fileClose(fp);
}
