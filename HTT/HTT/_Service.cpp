#include "all.h"

static autoList<char *> *CommandToArguments(char *command)
{
	char *p = command;
	char *q;
	autoList<char *> *dest = new autoList<char *>();

	while(*p)
	{
		if(*p == '"')
		{
			p++;
			q = strchr(p, '"');
			errorCase(!q);
			*q = '\0'; // 破壊
			dest->AddElement(strx(p));
			*q = '"'; // 復元
			q++;
		}
		else
		{
			q = strchr(p, ' ');

			if(!q)
			{
				dest->AddElement(strx(p));
				break;
			}
			*q = '\0'; // 破壊
			dest->AddElement(strx(p));
			*q = ' '; // 復元
			q++;
		}
		p = q;

		while(*p == ' ')
			p++;
	}
	errorCase(dest->GetCount() < 1); // 2bs

	for(int index = 0; index < dest->GetCount(); index++)
		cout("CTA_[%d]: \"%s\"\n", index, dest->GetElement(index));

	dest->AddElement(NULL);
	return dest;
}
int CheckArguments(autoList<char *> *args)
{
	char *arg;

	for(int index = 0; arg = args->GetElement(index); index++)
		for(char *p = arg; *p; p++)
			if(*p <= ' ')
				return 1;

	// _spawn 系に渡す場合 args を連結した文字列は 1024 バイトを超えてはならないらしい。
	{
		int count = -1;

		for(int index = 0; arg = args->GetElement(index); index++)
			count += 1 + strlen(arg);

		cout("command_length: %u\n", count);

		if(1024 < count)
			return 1;
	}
	return 0;
}

int ServiceArgsHasProblem;

Service_t *HTT_CreateService(char *name, char *command)
{
	errorCase(m_isEmpty(name));
	errorCase(m_isEmpty(command));

	for(char *p = name; *p; p++)
	{
		errorCase(!m_isRange(*p, 0x21, 0x7e));
	}

	Service_t *i = nb(Service_t);

	i->Name = strx(name);
	i->Command = strx(command);
	i->Arguments = CommandToArguments(i->Command);

	ServiceArgsHasProblem |= CheckArguments(i->Arguments);

	return i;
}
void HTT_ReleaseService(Service_t *i)
{
	memFree(i->Name);
	memFree(i->Command);
	releaseList(i->Arguments, (void (*)(char *))memFree);
	memFree(i);
}

autoList<Service_t *> *ServiceList;
int ServiceNameLenMin;
int ServiceNameLenMax;

#define SERVICENAMELENMAXMAX 1000

static int CompServiceName(Service_t *i1, Service_t *i2)
{
	return _stricmp(i1->Name, i2->Name);
}
void LoadServiceFile(char *serviceFile)
{
	FILE *fp = fileOpen(serviceFile, "rt");

	ServiceList = new autoList<Service_t *>();
	ServiceNameLenMin = SERVICENAMELENMAXMAX;
	ServiceNameLenMax = 0;

	for(; ; )
	{
		char *line = readLine(fp);

		if(!line)
			break;

		if(*line != ';') // ? not comment
		{
			char *p = strchr(line, ' ');
			errorCase(!p);
			*p++ = '\0';

			int len = strlen(line);
			m_minim(ServiceNameLenMin, len);
			m_maxim(ServiceNameLenMax, len);

			p = strx(p);

			{
				char *ESCPTN = "\n";
				char *basePath = getDir_x(getFullPath(serviceFile));
				basePath = combine_xc(basePath, ""); // "X:\\" -> "X:\\", "X:\\abc" -> "X:\\abc\\"

//				p = replace(p, "%%", ESCPTN); // old
//				p = replace(p, "%BASE%", basePath, 1); // old
				p = replace(p, "<BASE>", basePath, 1);
//				p = replace(p, ESCPTN, "%"); // old

				memFree(basePath);
			}

			ServiceList->AddElement(HTT_CreateService(line, p));
			memFree(p);
		}
		memFree(line);
	}
	fileClose(fp);

	errorCase(!ServiceList->GetCount()); // ? no services
	errorCase(ServiceNameLenMin < 1);
	errorCase(SERVICENAMELENMAXMAX < ServiceNameLenMax);
	errorCase(ServiceNameLenMax < ServiceNameLenMin);

	ServiceList->Sort(CompServiceName);

	for(int index = 1; index < ServiceList->GetCount(); index++) // サービス名の重複チェック
	{
		errorCase(!CompServiceName(
			ServiceList->GetElement(index - 1),
			ServiceList->GetElement(index)
			));
	}
	for(int index = 0; index < ServiceList->GetCount(); index++) // 表示
	{
		Service_t *s = ServiceList->GetElement(index);
		cout("%s ==> %s\n", s->Name, s->Command);
	}
	cout("ServiceArgsHasProblem: %d\n", ServiceArgsHasProblem);
}
Service_t *GetService(char *name)
{
	errorCase(!name);
	static Service_t ferret;
	ferret.Name = name;

	int index = ServiceList->BinSearch(CompServiceName, &ferret);

	if(index == -1) // ? not found
		return NULL;

	return ServiceList->GetElement(index);
}
