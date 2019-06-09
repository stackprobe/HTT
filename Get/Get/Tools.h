__int64 s2i64(char *line);
__int64 s2i64_x(char *line);
int s2i(char *line);

#define m_simpleComp(a, b) \
	((a) < (b) ? -1 : ((b) < (a) ? 1 : 0))
