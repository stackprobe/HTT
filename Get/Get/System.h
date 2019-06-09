void addFinalizer(void (*func)(void));

void termination(int errorlevel);
void error2(char *source, int lineno, char *function);

#define error() \
	error2(__FILE__, __LINE__, __FUNCTION__)

#define errorCase(status) \
	do { \
	if((status)) error(); \
	} while(0)

#define LOGPOS() \
	cout("%s (%d) %s\n", __FILE__, __LINE__, __FUNCTION__)

// sync > @ cout_h
void cout(char *format, ...);
char *xcout(char *format, ...);
char *vxcout(char *format, va_list marker);
void coutLongText(char *text);
void coutLongText_x(char *text);
// < sync

int argIs(char *spell);
char *getArg(int index);
char *nextArg(void);

char *getTimeStamp(time_t t = 0);
char *makeJStamp(char *eStamp);
char *c_makeJStamp(char *eStamp);

char *getSelfFile(void);
char *getSelfDir(void);
