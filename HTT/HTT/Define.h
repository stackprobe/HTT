#define _CRT_SECURE_NO_WARNINGS

// WinSock {
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wininet.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
// }

// 定番 {
#include <conio.h>
#include <ctype.h>
#include <direct.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <mbstring.h>
#include <process.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\types.h> // sys/stat.h より先であること。
#include <sys\stat.h>
#include <time.h>
// }

#include <windows.h>

typedef unsigned char uchar;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint;
typedef unsigned __int64 uint64;

#define lengthof(list) \
	(sizeof((list)) / sizeof(*(list)))

#define m_isEmpty(str) \
	(!(str) || !*(str))

#define m_min(value1, value2) \
	((value1) < (value2) ? (value1) : (value2))

#define m_max(value1, value2) \
	((value1) < (value2) ? (value2) : (value1))

#define m_minim(var, value) \
	((var) = m_min((var), (value)))

#define m_maxim(var, value) \
	((var) = m_max((var), (value)))

#define m_range(var, minval, maxval) \
	do { \
	m_maxim((var), (minval)); \
	m_minim((var), (maxval)); \
	} while(0)

#define m_isRange(value, minval, maxval) \
	((minval) <= (value) && (value) <= (maxval))

#define m_iSign(value) \
	((value) < 0 ? -1 : 0 < (value) ? 1 : 0)

#define m_d2i(value) \
	((int)((value) + 0.5))

#define m_accur(var, denom) \
	(m_d2i((var) * (denom)) / (denom))

#define m_pow(value) \
	((value) * (value))

/*
	ex.
		oneObject(int, (int *)na(int, 100), GetIntList)         ... プロトタイプ宣言は oneObjectProto(IntList, int)
		static oneObject(ClassABC, new ClassABC(), GetClassABC) ... static の場合
*/
#define oneObject(TYPE_T, init_op, getter) \
	TYPE_T *(getter)(void) { \
		static TYPE_T *p; \
		if(!p) { \
			p = (init_op); \
		} \
		return p; \
	}

#define oneObjectProto(TYPE_T, getter) \
	TYPE_T *(getter)(void);

#define m_hypot(x, y) \
	(sqrt(m_pow((double)(x)) * m_pow((double)(y))))

#define m_swap(v1, v2, TYPE_T) \
	do { \
	TYPE_T tmp = (v1); \
	(v1) = (v2); \
	(v2) = tmp; \
	} while(0)

typedef struct i2D_st
{
	int X;
	int Y;
}
i2D_t;

#define IMAX 1000000000
