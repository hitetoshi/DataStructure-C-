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
	if (pos<0 || pos>=s->length || len<0 || len>s->length-pos)
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

	i = pos;
	j = 0;
	//时间复杂度受指针回退次数影响
	//最优O(s->length+t->length)
	//最坏O(s->length*t->length)
	while (i < s->length && j < t->length)
	{
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
		if (j == -1 || s->ch[i] == t->ch[j])
		{
			//继续比较后继字节
			i++;
			j++;
		}
		else
		{
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

static size_t g_next[1024];

size_t StrIndexKMPOpt(HString *s, HString *t, size_t pos)
{
	size_t i, j;

	KMP_NextVal(t, g_next);
	i = pos;
	j = 0;
	while (i < s->length && (j < t->length || j == -1))
	{
		if (j == -1 || s->ch[i] == t->ch[j])
		{
			//继续比较后继字节
			i++;
			j++;
		}
		else
		{
			//指针回退
			if (j == 0)
			{
				j--;
			}
			else
			{
				//内存访问优化,使用全局数组而非动态分配数组
				//CPU将采用间接寻址而非基址变址寻址访问next表内容
				j = g_next[j];
			}
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

Status StrReplace(HString *s, HString *t, HString *v)
{
	size_t pos=0;

	while (pos = StrIndex(s, t, pos))
	{
		if (StrDelete(s, pos, t->length) != OK)
		{
			return ERROR;
		}
		if (StrInsert(s, pos, v) != OK)
		{
			return ERROR;
		}
	}
	return OK;
}

Status StrDelete(HString *s, size_t pos, size_t len)
{
	if (pos<0 || pos>=s->length || pos + len >= s->length)
	{
		return ERROR;
	}
	CMEM_MOVE(s->ch + pos, s->ch + pos + len, len);
	s->length -= len;
	s->ch[s->length] = 0;
	return OK;
}

HSElem *StrPointer(HString *t)
{
	return t->ch;
}

HSElem StrElem(HString *t, size_t pos)
{
	return (pos >= 0 && pos < t->length) ? t->ch[pos] : 0;
}

//关键词索引表

char *book_info[] =
{
	"005 Computer Data Structures",
	"010 Introduction to Data Structures",
	"023 Fundamentals of Data Structures",
	"034 The Design and Analysis of Computer Algorithms",
	"050 Introduction to Numberical Analysis",
	"067 Numberical Analysis"
};

char *commonly_word[] =
{
	"an","a","of","the","to","and"
};

int __cdecl IdxWordlistCompare(ELEMENT *first, ELEMENT *second)
{
	return StrCompare((HString *)first->data, (HString *)second->data);
}

Status ExtractKeyWord(char *buf, SQLIST *commwdlist, SQLIST *wdlist, int *BookNo)
{
	int count = 0;
	ELEMENT e;
	HString str;
	size_t pos;
	size_t start;
	HString blank;
	HString wd;
	Status ret = ERROR;

	StrInit(&str);
	StrInit(&blank);
	StrAssign(&str, buf);
	CSTRING_LOWER(StrPointer(&str));	//字符串转换为小写
	StrAssign(&blank, " ");
	pos=StrIndex(&str, &blank, 0);
	if (pos)
	{
		StrInit(&wd);
		e.data = &wd;
		e.size = sizeof(wd);
		if (StrSubString(&wd, &str, 0, pos) == OK)
		{
			*BookNo = CSTRING_ATOI(StrPointer(&wd));
			StrDestroy(&wd);
			start = ++pos;
			do
			{
				pos = StrIndex(&str, &blank, pos);
				if (pos == -1)
				{
					pos = StrLength(&str);
				}
				StrInit(&wd);
				e.data = &wd;
				e.size = sizeof(wd);
				if (StrSubString(&wd, &str, start, pos - start) == OK)
				{
					if (SqlistLocate(commwdlist, &e) == -1)	//排除常用词
					{
						SqlistOrderInsert(wdlist, &e);
					}
				}
				else
				{
					StrDestroy(&wd);
				}
				start = ++pos;
			} while (start<StrLength(&str));
			ret = OK;
		}
		else
		{
			StrDestroy(&wd);
		}
	}
	StrDestroy(&str);
	StrDestroy(&blank);
	return ret;
}

//初始化常用词表
void InitCommonlyWorlist(SQLIST *commwdlist)
{
	ELEMENT e;
	size_t i;
	HString s;

	for (i = 0; i < sizeof(commonly_word) / sizeof(char *); i++)
	{
		StrInit(&s);
		e.data = &s;
		e.size = sizeof(s);
		StrAssign(&s, commonly_word[i]);
		CSTRING_LOWER(StrPointer(&s));
		SqlistOrderInsert(commwdlist, &e);
	}
}

Status __cdecl free_hstring_list(ELEMENT *elem)
{
	StrDestroy((HString *)elem->data);
	return OK;
}

void Freewdlist(SQLIST *wdlist)
{
	SqlistTraverse(wdlist, free_hstring_list);
}

int __cdecl IdxListCompare(ELEMENT *first, ELEMENT *second)
{
	return StrCompare(&((IDXTERMTYPE *)first->data)->key, &((IDXTERMTYPE *)second->data)->key);
}

Status InitIdxList(SQLIST *idxlist)
{
	if (SqlistInit(idxlist, IdxListCompare, CommonAllocRotuine, CommonFreeRoutine) != OK)
	{
		return ERROR;
	}
	return OK;
}

Status GetWord(SQLIST *wdlist, size_t i, HString **wd)
{
	ELEMENT e;

	if (SqlistGetElem(wdlist, i, &e) == OK)
	{
		*wd = e.data;
		return OK;
	}
	return ERROR;
}

Status InsertNewKey(SQLIST *idxlist, size_t i, HString *wd)
{
	Status status = ERROR;
	IDXTERMTYPE idxelem;
	ELEMENT e;
	//教材重新定义了索引表数据类型,很多算法需要重新实现在插入索引表项之前需要先后移索引表其它项
	//本例使用已经封装好的线性顺序表,直接调用其Insert操作即可
	//可以使用SqlistInsert或SqlistOrderInsert,由于教材参数多给以个位置i,本例使用SqlistInsert
	//由代码上下文可知,i本身也是有序的位置序号
	StrInit(&idxelem.key);
	StrCopy(&idxelem.key, wd);
	if (LinklistInit(&idxelem.bnolist,
		CommonIntCompareRoutine,		//书号链表的类型为int,可使用通用整型比较函数
		CommonAllocRotuine, CommonFreeRoutine) == OK)
	{
		//有序链表需要为头结点分配内存
		idxelem.bnolist.header->elem.data = CMEM_ALOC(sizeof(int));
		if (idxelem.bnolist.header->elem.data)
		{
			idxelem.bnolist.header->elem.size = sizeof(int);
			*((int *)idxelem.bnolist.header->elem.data) = -1;

			e.data = &idxelem;
			e.size = sizeof(idxelem);
			status = SqlistInsert(idxlist, i, &e);
			if (status != OK)
			{
				StrDestroy(&idxelem.key);
				CMEM_FREE(idxelem.bnolist.header->elem.data);
				LinklistDestroy(&idxelem.bnolist);
			}
		}
		else
		{
			StrDestroy(&idxelem.key);
			LinklistDestroy(&idxelem.bnolist);
		}
	}
	else
	{
		StrDestroy(&idxelem.key);
	}
	return status;
}

Status InsertBook(SQLIST *idxlist, size_t i, int bno)
{
	ELEMENT elist;
	ELEMENT ebook;

	//与教材算法4.14不同,直接调用链表的LinklistOrderInsert操作插入数据
	if (SqlistGetElem(idxlist, i, &elist) == OK)
	{
		ebook.data = &bno;
		ebook.size = sizeof(bno);
		return LinklistOrderInsert(&((IDXTERMTYPE *)elist.data)->bnolist, &ebook);
	}
	return ERROR;
}

Status InsertIdxList(SQLIST *idxlist, int bno, SQLIST *wdlist)
{
	HString *wd;
	size_t i;
	size_t pos;
	ELEMENT e, *b;
	IDXTERMTYPE idx;

	StrInit(&idx.key);

	for (i = 0; i < SqlistLength(wdlist); i++)
	{
		if (GetWord(wdlist, i, &wd) == OK)
		{
			StrCopy(&idx.key, wd);
			e.data = &idx;
			e.size = sizeof(idx);
			//有序查找给定关键词在关键词索引中是否存在
			//注意:教材用算法4.12实现查找,在第3个参数(Boolean类型)中返回元素是否存在
			//除非在头文件中包含stdbool.h,否则Boolean类型是没有定义的
			//本例直接使用有序链表的SqlistOrderLocate操作,第3个参数b返回这个元素,若元素不存在,则b为NULL
			b = NULL;
			pos = SqlistOrderLocate(idxlist, &e, &b);
			if (!b)
			{
				if (InsertNewKey(idxlist, pos, wd) != OK)
				{
					continue;
				}
			}
			InsertBook(idxlist, pos, bno);
		}
	}

	StrDestroy(&idx.key);
	return OK;
}

Status CreateIdxList(SQLIST *idxlist)
{
	SQLIST wdlist;		//词表
	SQLIST commwdlist;	//常用词表
	int BookNo;			//书号
	size_t i;
	char *buf;

	if (!SqlistInit(&wdlist, IdxWordlistCompare, CommonAllocRotuine, CommonFreeRoutine) == OK)
	{
		return ERROR;
	}
	if (!SqlistInit(&commwdlist, IdxWordlistCompare, CommonAllocRotuine, CommonFreeRoutine) == OK)
	{
		SqlistDestroy(&wdlist);
		return ERROR;
	}

	InitCommonlyWorlist(&commwdlist);
	InitIdxList(idxlist);

	//本例从内存中早入图书信息,与教材从文件中载入没有太大的区别
	for (i = 0; i < sizeof(book_info) / sizeof(char *); i++)
	{
		buf = book_info[i];								//依次读取书目信息到缓冲区buf
														//相当于算法4.9的GetLine
		SqlistClear(&wdlist);
		if (ExtractKeyWord(buf, &commwdlist, &wdlist, &BookNo) == OK)	//从buf提取关键词到词表,书号存入BookNo
		{
			InsertIdxList(idxlist, BookNo, &wdlist);	//将书号为BookNo的关键词插入索引表
		}
	}
	//本例不将idxlist保存为文件,而是返回给调用者,以便调用者后续进行检索操作

	Freewdlist(&commwdlist);
	Freewdlist(&wdlist);
	SqlistDestroy(&wdlist);
	SqlistDestroy(&commwdlist);
	return OK;
}


Status __cdecl traverse_free_idxlist(ELEMENT *elem)
{
	//释放关键词串
	StrDestroy(&((IDXTERMTYPE *)elem->data)->key);
	//释放书号链表
	CMEM_FREE(((IDXTERMTYPE *)elem->data)->bnolist.header->elem.data);
	LinklistDestroy(&((IDXTERMTYPE *)elem->data)->bnolist);
}

void DestroyIdxList(SQLIST *idxlist)
{
	SqlistTraverse(idxlist, traverse_free_idxlist);
	SqlistDestroy(idxlist);
}