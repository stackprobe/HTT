extern char *DefaultMimeType;
extern char *MimeList[][2];

void Debug_GenerateMimeTypeFile();

char *GetMimeType(char *file);
int IsMimeHtml(char *file);
