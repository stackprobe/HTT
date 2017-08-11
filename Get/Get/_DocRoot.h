typedef struct DocRoot_st
{
	char *Domain;
	char *RootDir;
}
DocRoot_t;

void LoadDocRootFile(char *docRootFile);
DocRoot_t *GetDocRoot(char *domain);
