#include "all.h"

autoList<DocRoot_t *> *DocRootList;

static DocRoot_t *CreateDocRoot(char *domain, char *rootDir, char *baseDir)
{
	errorCase(m_isEmpty(domain));
	errorCase(m_isEmpty(rootDir));

	DocRoot_t *i = nb(DocRoot_t);

	i->Domain = strx(domain);
	i->RootDir = getFullPath(rootDir, baseDir);

	return i;
}
void LoadDocRootFile(char *docRootFile)
{
	char *baseDir = getDir(docRootFile);
	FILE *fp = fileOpen(docRootFile, "rt");

	DocRootList = new autoList<DocRoot_t *>();

	for(; ; )
	{
		char *line = readLine(fp);

		if(!line)
			break;

		if(line[0] != ';') // ? not comment
		{
			char *p = strchr(line, ' ');
			errorCase(!p);
			*p++ = '\0';
			DocRootList->AddElement(CreateDocRoot(line, p, baseDir));
		}
		memFree(line);
	}
	fileClose(fp);
	memFree(baseDir);
}
DocRoot_t *GetDocRoot(char *domain)
{
	for(int index = 0; index < DocRootList->GetCount(); index++)
	{
		DocRoot_t *dr = DocRootList->GetElement(index);

		if(!_stricmp(domain, dr->Domain))
		{
			return dr;
		}
	}
	return NULL;
}
