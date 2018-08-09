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

#ifdef __cplusplus
}
#endif // __cplusplus

