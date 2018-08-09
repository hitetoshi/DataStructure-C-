#include "HString.h"

Status StrAssign(HString *t, char *chars)
{
	size_t i;
	if (t->ch)
	{
		CMEM_FREE(t->ch);
	}
	i = CSTRING_LENGTH(chars);
	if (!i)
	{
		t->ch = NULL;
		t->length = 0;
	}
	else
	{
		t->ch = CMEM_ALOC(i);
		if (!t->ch)
		{
			return OVERFLOW;
		}
		CMEM_COPY(t->ch, chars, i);
		t->length = i;
	}
	return OK;
}