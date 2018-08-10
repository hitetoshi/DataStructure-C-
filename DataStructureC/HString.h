#pragma once

#include "DataStructureC.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	typedef	struct
	{
		char *ch;	//���Ƿǿմ�,�򰴴����ȷ���洢��,����chΪNULL
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

