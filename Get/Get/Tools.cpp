#include "all.h"

__int64 s2i64(char *line)
{
	__int64 result = 0;
	int sign = 1;

	for(char *p = line; *p; p++)
	{
		int digit = -1;

		if(*p == '-')
		{
			sign = -1;
		}
		else if(*p == '\x82') // "‚O" == { '\x82', '\x4f' }
		{
			p++;

			if(!*p)
				break;

			if('\x4f' <= *p && *p <= '\x58')
			{
				digit = *p - 0x4f;
			}
		}
		else if('0' <= *p && *p <= '9')
		{
			digit = *p - '0';
		}
		if(0 <= digit)
		{
			if((_I64_MAX - 9) / 10 < result)
				break;

			result *= 10;
			result += digit;
		}
	}
	return result * sign;
}
__int64 s2i64_x(char *line)
{
	__int64 value = s2i64(line);
	memFree(line);
	return value;
}
int s2i(char *line)
{
	__int64 value = s2i64(line);
	m_range(value, -INT_MAX, INT_MAX);
	return (int)value;
}
