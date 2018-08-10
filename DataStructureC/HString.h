#pragma once

#include "DataStructureC.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	typedef	struct
	{
		char *ch;	//若是非空串,则按串长度分配存储区,否则ch为NULL
		size_t length;
	}HString;

	Status StrAssign(HString *t, char *chars);
	size_t StrLength(HString *s);
	int StrCompare(HString *s, HString *t);
	Status StrClear(HString *s);
	Status StrConcat(HString *t, HString *s1, HString *s2);

#ifdef __cplusplus
}
#endif // __cplusplus

