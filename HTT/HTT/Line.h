char *strx(char *line);
char *strr(char *line);
void strz(char *&buffer, char *line);
void strz_x(char *&buffer, char *line);

char *getConstNullString(void);
char *getNullString(void);

autoList<char *> *tokenize(char *line, char *delimiters);
char *untokenize(autoList<char *> *tokens, char *separator);

void copyBlock(void *dest, void *src, int size);
void copyLine(char *dest, char *src);

void trimLead(char *line, int chr);
void trimTrail(char *line, int chr);
void trimEdge(char *line, int chr);
void trimSequ(char *line, int delimChr);
void trim(char *line, int chr);

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
char *replace(char *str, char *srcPtn, char *destPtn, int ignoreCase = 0);
char *makeUpper(char *str);

char *addToken(char *buffer, char *token);
char *addToken_x(char *buffer, char *token);
char *addChar(char *buffer, int chr);
char *addLine(char *buffer, char *line);
char *addLine_x(char *buffer, char *line);

char *getEnd(char *str, int num);
char *getEnd(char *str, char *ptn);
