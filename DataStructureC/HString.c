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

size_t StrLength(HString *s)
{
	return s->length;
}

int StrCompare(HString *s, HString *t)
{
	size_t i;

	for (i = 0; i < s->length && i < t->length; i++)
	{
		if (s->ch[i] != t->ch[i])
		{
			return s->ch[i] - t->ch[i];
		}
	}
	return (int)(s->length - t->length);
}

Status StrClear(HString *s)
{
	if (s->ch)
	{
		CMEM_FREE(s->ch);
		s->ch = NULL;
	}
	s->length = 0;
	return OK;
}

Status StrConcat(HString *t, HString *s1, HString *s2)
{

}