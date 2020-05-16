typedef struct HeaderField_st
{
	char *Name;
	char *Value;
}
HeaderField_t;

extern char *HRH_Header;
extern autoList<HeaderField_t *> *HRH_Fields;

int LoadHTTPRequestHeader(char *file, size_t fileSizeLimit, char *loadedHTTPRequestHeaderFile);

extern int HRH_UrlEndSlash;
extern autoList<char *> *HRH_UrlPaths;
extern char *HRH_UserAgent;
extern char *HRH_HostName;
extern char *HRH_HostPortPart;
extern int HRH_KeepAlive;

int ParseHTTPRequestHeader(void);
