#include "all.h"

#define DEFAULT_MIME_TYPE "application/octet-stream"

char *GetMimeType(char *file)
{
	char *ext = getExt(file);
	char *mimeTypeFile = combine(getSelfDir(), MIME_TYPE_FILE); // g
	autoList<char *> *lines = readLines(mimeTypeFile); // g

	for(int index = 0; index < lines->GetCount(); index++)
	{
		char *line = lines->GetElement(index);
		char *p = strchr(line, '\t');

		errorCase(!p);

		*p = '\0';

		if(!_stricmp(line, ext))
		{
			return p + 1; // íçà”ÅFp ÇÕ lines ÇÃàÍïî
		}
	}
	return DEFAULT_MIME_TYPE;
}
int IsMimeHtml(char *file)
{
	char *ext = getExt(file);

	return
		!_stricmp(ext, "htm") ||
		!_stricmp(ext, "html");
}
