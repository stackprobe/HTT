typedef struct Service_st
{
	char *Name;
	char *Command;
	autoList<char *> *Arguments;
}
Service_t;

extern int ServiceArgsHasProblem;

Service_t *HTT_CreateService(char *name, char *execFile);

extern autoList<Service_t *> *ServiceList;
extern int ServiceNameLenMin;
extern int ServiceNameLenMax;

void LoadServiceFile(char *adapterFile);
Service_t *GetService(char *name);
