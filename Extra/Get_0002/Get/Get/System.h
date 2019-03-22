void addFinalizer(void (*func)(void));
void (*unaddFinalizer(void))(void);

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

int hasArgs(int count);
int argIs(char *spell);
char *getArg(int index);
char *nextArg(void);
int getArgIndex(void);
void setArgIndex(int index);

char *makeTempPath(char *suffix = ".tmp");
char *makeTempFile(char *suffix = ".tmp");
char *makeTempDir(char *suffix = ".tmp");

double now(void);
char *getTimeStamp(time_t t = 0);
char *makeJStamp(char *eStamp);
char *c_makeJStamp(char *eStamp);

char *getSelfFile(void);
char *getSelfDir(void);
