char *strx(char *line);
char *strr(char *line);
void strz(char *&buffer, char *line);

void trimSequ(char *line, int delimChr);

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

int strcmp_xc(char *a, char *b);
