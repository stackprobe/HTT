char *strx(char *line);
char *strr(char *line);
void strz(char *&buffer, char *line);

autoList<char *> *tokenize(char *line, char *delimiters = "\t ");
autoList<char *> *tokenize_xc(char *line, char *delimiters = "\t ");
char *untokenize(autoList<char *> *tokens, char *separator = " ");
char *untokenize_xc(autoList<char *> *tokens, char *separator = " ");

void copyBlock(void *dest, void *src, int size);
void copyLine(char *dest, char *src);

void trimLead(char *line, int chr = ' ');
void trimTrail(char *line, int chr = ' ');
void trimEdge(char *line, int chr = ' ');
void trimSequ(char *line, int delimChr = ' ');
void trim(char *line, int chr = ' ');
void unctrl(char *line);

#define isMbc1(chr) \
	(_ismbblead((chr)))

#define isMbc2(chr) \
	(_ismbbtrail((chr)))

#define mbcNext(p) \
	((p) + (isMbc1(*(p)) && (p)[1] ? 2 : 1))

#define mbcInc(p) \
	((p) = mbcNext((p)))

void replaceMbsChar(char *line, int chr1, int chr2);
void escapeYen(char *line);
void unescapeYen(char *line);

char *addToken(char *buffer, char *token);
char *addChar(char *buffer, int chr);
char *addLine(char *buffer, char *line);

char *getEnd(char *str, int num);

int c2lc(int chr);

int my_strnicmp(char *str1, char *str2, int count);

int isRangeToken(char *token, int minchr, int maxchr);
int isAsciiToken(char *token);
void toAsciiToken(char *token);
void toJToken(char *token);
