#include "all.h"

static oneObject(autoList<void (*)(void)>, new autoList<void (*)(void)>(), GetFinalizers)

void addFinalizer(void (*func)(void))
{
	GetFinalizers()->AddElement(func);
}
void (*unaddFinalizer(void))(void)
{
	return GetFinalizers()->UnaddElement();
}

void termination(int errorlevel)
{
	while(GetFinalizers()->GetCount())
	{
		GetFinalizers()->UnaddElement()();
	}
	if(RemoveSendFileAtTermination)
	{
		LOGPOS();
		_fcloseall();
		remove(SendFileFullPath);
	}
	exit(errorlevel);
}
void error2(char *source, int lineno, char *function)
{
	{
		static int passed;

		if(passed)
			exit(2);

		passed = 1;
	}

	cout("GET.ERROR %s %d %s\n", source, lineno, function);

//	MessageBox(NULL, xcout("Error %s %d %s", source, lineno, function), "HTT.Get Error", MB_OK | MB_ICONSTOP | MB_TOPMOST);

	// app >

	HTT_Disconnect();

	// < app

	termination(1);
}

// sync > @ cout

void cout(char *format, ...)
{
	va_list marker;

	va_start(marker, format);

	if(!strcmp(format, "%s"))
	{
		coutLongText(va_arg(marker, char *));
	}
	else if(!strcmp(format, "%s\n"))
	{
		coutLongText(va_arg(marker, char *));
		coutLongText("\n");
	}
	else
		coutLongText_x(vxcout(format, marker));

	va_end(marker);
}
char *xcout(char *format, ...)
{
	char *ret;
	va_list marker;

	va_start(marker, format);
	ret = vxcout(format, marker);
	va_end(marker);

	return ret;
}
char *vxcout(char *format, va_list marker)
{
	char *buffer;

	for(int size = strlen(format) + 100; ; size *= 2)
	{
		int ret;

#define MARGIN 10

		buffer = (char *)memAlloc(size + MARGIN * 2);
		ret = _vsnprintf(buffer, size + MARGIN, format, marker);

#undef MARGIN

		if(0 <= ret && ret <= size)
			break;

		memFree(buffer);
		errorCase(128 * 1024 * 1024 < size); // ANTI OVER-FLOW
	}
	return strr(buffer);
}
void coutLongText(char *text)
{
	coutLongText_x(strx(text));
}
void coutLongText_x(char *text)
{
	char *p;
	char *q;

	for(p = text; *p; p = q)
	{
		int bkc;

#define FPUTS_TEXT_LMT 100

		for(q = p; *q && ((uint)q - (uint)p) < FPUTS_TEXT_LMT; q += _ismbblead(*q) ? 2 : 1)
		{}

#undef FPUTS_TEXT_LMT

		bkc = *q;
		*q = '\0';
		errorCase(fputs(p, stdout) < 0);
		*q = bkc;
	}
	memFree(text);
}

// < sync

static int ArgIndex = 1;

int hasArgs(int count)
{
	return count <= __argc - ArgIndex;
}
int argIs(char *spell)
{
	if(ArgIndex < __argc)
	{
		if(!_stricmp(__argv[ArgIndex], spell))
		{
			ArgIndex++;
			return 1;
		}
	}
	return 0;
}
char *getArg(int index)
{
	errorCase(index < 0 || __argc - ArgIndex <= index);
	return __argv[ArgIndex + index];
}
char *nextArg(void)
{
	char *arg = getArg(0);

	ArgIndex++;
	return arg;
}
int getArgIndex(void)
{
	return ArgIndex;
}
void setArgIndex(int index)
{
	errorCase(index < 0 || __argc < index); // index == __argc ÇÕëSïîì«Ç›èIÇÌÇ¡ÇΩèÛë‘
	ArgIndex = index;
}

char *makeTempPath(char *suffix)
{
error(); // unimpl
	return NULL;
}
char *makeTempFile(char *suffix)
{
	char *out = makeTempPath(suffix);
	createFile(out);
	return out;
}
char *makeTempDir(char *suffix)
{
	char *out = makeTempPath(suffix);
	createDir(out);
	return out;
}

double now(void)
{
	return clock() / (double)CLOCKS_PER_SEC;
}
char *getTimeStamp(time_t t) // t: 0 == åªéûçè
{
	static char timeStamp[25];
	char *p;

	if(!t)
		t = time(NULL);

	p = ctime(&t); // "Wed Jan 02 02:03:55 1980\n"

	if(!p) // ? maybe invalid t
		p = "Thu Jan 01 00:00:00 1970";

	memcpy(timeStamp, p, 24);
	return timeStamp; // "Wed Jan 02 02:03:55 1980"
}
/*
	"Wed Jan 02 02:03:55 1980" -> "1980/01/02 (êÖ) 02:03:55"
*/
char *makeJStamp(char *eStamp)
{
	autoList<char *> *dtParts = tokenize(eStamp, " ");
	errorCase(dtParts->GetCount() != 5);
	autoList<char *> *timeParts = tokenize(dtParts->GetElement(3), ":");
	errorCase(timeParts->GetCount() != 3);

	int y = s2i(dtParts->GetElement(4));
	int m = -1;
	int d = s2i(dtParts->GetElement(2));
	int h = s2i(timeParts->GetElement(0));
	int n = s2i(timeParts->GetElement(1));
	int s = s2i(timeParts->GetElement(2));
	int w = -1;

	const char *months[] =
	{
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", // "Dec",
	};
	const char *weeks[] =
	{
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", // "Sat",
	};
	const char *jWeeks[] =
	{
		"ì˙", "åé", "âŒ", "êÖ", "ñÿ", "ã‡", "ìy",
//		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
	};

	for(m = 1; m < 12; m++)
		if(!strcmp(months[m - 1], dtParts->GetElement(1)))
			break;

	for(w = 1; w < 7; w++)
		if(!strcmp(weeks[w - 1], dtParts->GetElement(0)))
			break;

	errorCase(!m_isRange(y, 0, 9999));
	errorCase(!m_isRange(m, 1, 12));
	errorCase(!m_isRange(d, 1, 31));
	errorCase(!m_isRange(h, 0, 23));
	errorCase(!m_isRange(n, 0, 59));
	errorCase(!m_isRange(s, 0, 59));
	errorCase(!m_isRange(w, 1, 7));

	return xcout(
		"%04d/%02d/%02d (%s) %02d:%02d:%02d"
		,y
		,m
		,d
		,jWeeks[w - 1]
		,h
		,n
		,s
		);
}
char *c_makeJStamp(char *eStamp)
{
	static char *stock;
	memFree(stock);
	return stock = makeJStamp(eStamp);
}

char *getSelfFile(void)
{
	static char *fileBuff;

	if(!fileBuff)
	{
		const int BUFFSIZE = 1000;
		const int MARGINSIZE = 10;

		fileBuff = (char *)memAlloc(BUFFSIZE + MARGINSIZE);
		errorCase(!GetModuleFileName(NULL, fileBuff, BUFFSIZE)); // ? é∏îs
	}
	return fileBuff;
}
char *getSelfDir(void)
{
	static char *dirBuff;

	if(!dirBuff)
		dirBuff = getDir(getSelfFile());

	return dirBuff;
}
