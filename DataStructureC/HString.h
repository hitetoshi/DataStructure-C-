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

#ifdef __cplusplus
}
#endif // __cplusplus

