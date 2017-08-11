#include "all.h"

int getZero(void)
{
	return 0;
}
int s2i(char *line)
{
	int result = 0;
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
			if((INT_MAX - 9) / 10 < result)
				break;

			result *= 10;
			result += digit;
		}
	}
	return result * sign;
}

int compInt(int a, int b)
{
	if(a < b) return -1;
	if(b < a) return 1;
	return 0;
}
int compI2D(i2D_t a, i2D_t b)
{
	int retval = compInt(a.X, b.X);

	if(!retval)
		retval = compInt(a.Y, b.Y);

	return retval;
}

int getHitKey(int defaultKey)
{
	int key = defaultKey;

	if(_kbhit())
	{
		cout("stdin ");
		key = _getch();
		cout("%d\n", key);
	}
	return key;
}

double dPow(double value, int extend)
{
	errorCase(extend < 0);

	if(extend == 0)
		return 1.0;

	double answer = value;

	while(0 < --extend)
		answer *= value;

	return answer;
}
