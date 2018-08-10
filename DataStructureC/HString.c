#include "HString.h"

void StrInit(HString *t)
{
	t->ch = CMEM_ALOC(sizeof(HSElem));
	if (t->ch)
	{
		t->ch[0] = 0;
	}
	t->length = 0;
}

Status StrAssign(HString *t, HSElem *chars)
{
	size_t i;
	if (t->ch)
	{
		CMEM_FREE(t->ch);
	}
	i = HSElem_Len(chars);
	if (!i)
	{
		t->ch = NULL;
		t->length = 0;
	}
	else
	{
		t->ch = CMEM_ALOC((i+1)*sizeof(HSElem));
		if (!t->ch)
		{
			return OVERFLOW;
		}
		CMEM_COPY(t->ch, chars, i*sizeof(HSElem));
		t->length = i;
		t->ch[t->length] = 0;
	}
	return OK;
}

Status StrCopy(HString *t, HString *s)
{
	if (t->ch)
	{
		CMEM_FREE(t->ch);
	}
	t->ch = CMEM_ALOC((s->length + 1) * sizeof(HSElem));
	if (t->ch)
	{
		CMEM_COPY(t->ch, s->ch, s->length * sizeof(HSElem));
		t->length = s->length;
		t->ch[t->length] = 0;
		return OK;
	}
	else
	{
		return OVERFLOW;
	}
}

Status StrEmpty(HString *s)
{
	return (s->length == 0);
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
		CMEM_SET(s->ch, 0, sizeof(HSElem)*(s->length + 1));
	}
	s->length = 0;
	return OK;
}

Status StrDestroy(HString *s)
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
	if (t->ch)
	{
		CMEM_FREE(t->ch);
	}
	t->ch = CMEM_ALOC((s1->length + s2->length + 1)*sizeof(HSElem));
	if (!t->ch)
	{
		return OVERFLOW;
	}
	CMEM_COPY(t->ch, s1->ch, s1->length*sizeof(HSElem));
	CMEM_COPY(t->ch + s1->length, s2->ch, s2->length*sizeof(HSElem));
	t->length = s1->length + s2->length;
	t->ch[t->length] = 0;
	return OK;
}

Status StrCat(HString *t, HString *s)
{
	HSElem *new_buffer;

	if (t->ch)
	{
		new_buffer = CMEM_REALLOC(t->ch, (t->length + s->length + 1) * sizeof(HSElem));
		if (!new_buffer)
		{
			return OVERFLOW;
		}
		t->ch = new_buffer;
	}
	else
	{
		t->ch = CMEM_ALOC((t->length + s->length + 1) * sizeof(HSElem));
		return OVERFLOW;
	}
	CMEM_COPY(t->ch + t->length, s->ch, s->length);
	t->length += s->length;
	t->ch[t->length] = 0;
	return OK;
}

Status StrCatc(HString *t, HSElem c)
{
	HSElem *new_buffer;

	if (t->ch)
	{
		new_buffer = CMEM_REALLOC(t->ch, (t->length + 1 + 1) * sizeof(HSElem));
		if (!new_buffer)
		{
			return OVERFLOW;
		}
		t->ch = new_buffer;
	}
	else
	{
		t->ch = CMEM_ALOC((t->length + 1 + 1) * sizeof(HSElem));
		return OVERFLOW;
	}
	t->ch[t->length++] = c;
	t->ch[t->length] = 0;
	return OK;
}

Status StrSubString(HString *sub, HString *s, size_t pos, size_t len)
{
	if (pos<0 || pos>=s->length || len<0 || len>=s->length-pos)
	{
		return ERROR;
	}
	if (sub->ch)
	{
		CMEM_FREE(sub->ch);
	}
	sub->ch = NULL;
	sub->length = 0;
	if (!len)
	{
		return OK;
	}
	else
	{
		sub->ch = CMEM_ALOC((len + 1) * sizeof(HSElem));
		if (!sub->ch)
		{
			return OVERFLOW;
		}
		CMEM_COPY(sub->ch, s->ch + pos, len*sizeof(HSElem));
		sub->length = len;
		sub->ch[sub->length] = 0;
		return OK;
	}
}

Status StrInsert(HString *s, size_t pos, HString *t)
{
	HSElem *new_buffer;

	if (pos<0 || pos>=s->length)
	{
		return ERROR;
	}
	if (t->length)
	{
		//与教材的代码稍有不同,realloc分配失败返回NULL,不修改原来的内存块,
		//若按照教材上的写法,当realloc返回NULL时,s->ch被赋值为NULL,导致
		//s->ch原来分指向的内存块丢失,无法释放
		new_buffer = CMEM_REALLOC(s->ch, (s->length + t->length + 1) * sizeof(HSElem));
		if (!new_buffer)
		{
			return OVERFLOW;
		}
		s->ch = new_buffer;
		CMEM_MOVE(s->ch + pos + t->length, s->ch + pos, (s->length - pos) * sizeof(HSElem));
		CMEM_COPY(s->ch + pos, t->ch, t->length * sizeof(HSElem));
		s->length += t->length;
		s->ch[s->length] = 0;
	}
	return OK;
}

size_t StrIndex(HString *s, HString *t, size_t pos)
{
	size_t i, j;
	int compare_count = 0;
	int back_count = 0;

	i = pos;
	j = 0;
	//时间复杂度受指针回退次数影响
	//最优O(s->length+t->length)
	//最坏O(s->length*t->length)
	while (i < s->length && j < t->length)
	{
		compare_count++;
		if (s->ch[i] == t->ch[j])
		{
			//继续比较后继字节
			i++;
			j++;
		}
		else
		{
			//指针后退重新开始匹配
			i = i - j + 1;
			j = 0;
			back_count++;
		}
	}
	if (j >= t->length)
	{
		return i - t->length;
	}
	else
	{
		return -1;
	}
}

void KMP_Next(HString *t, size_t next[])
{
	size_t i = 0;
	size_t j = -1;

	next[0] = -1;

	while (i < t->length - 1)
	{
		if (j == -1 || t->ch[i] == t->ch[j])
		{
			i++;
			j++;
			next[i] = j;
		}
		else
		{
			j = next[j];
		}
	}
}

void KMP_NextVal(HString *t, size_t next[])
{
	size_t i = 0;
	size_t j = -1;

	next[0] = -1;

	while (i < t->length - 1)
	{
		if (j == -1 || t->ch[i] == t->ch[j])
		{
			i++;
			j++;
			next[i] = (t->ch[i] != t->ch[j]) ? j : next[j];
		}
		else
		{
			j = next[j];
		}
	}
}

size_t StrIndexKMP(HString *s, HString *t, size_t pos)
{
	size_t i, j;
	size_t *next;
	int compare_count = 0;
	int back_count = 0;

	next = CMEM_ALOC(sizeof(size_t)*(t->length));
	if (!next)
	{
		return -1;
	}
	//KMP_Next(t, next);
	KMP_NextVal(t, next);
	i = pos;
	j = 0;
	while (i < s->length && (j < t->length || j == -1))
	{
		compare_count++;
		if (j == -1 || s->ch[i] == t->ch[j])
		{
			//继续比较后继字节
			i++;
			j++;
		}
		else
		{
			back_count++;
			//指针回退
			j = next[j];
		}
	}
	if (j >= t->length)
	{
		CMEM_FREE(next);
		return i - t->length;
	}
	else
	{
		CMEM_FREE(next);
		return -1;
	}
}

HSElem *StrPointer(HString *t)
{
	return t->ch;
}

HSElem StrElem(HString *t, size_t pos)
{
	return (pos >= 0 && pos < t->length) ? t->ch[pos] : 0;
}