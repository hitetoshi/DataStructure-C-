#pragma once

#include "DataStructureC.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	//本例对教材的堆分配结构串作了一些修改
	//在分配串存储空间时,多分配1个字符的长度
	//在串最末尾存储'\0',这样可以让C/C++方便
	//地使用我们的串结构
	
	//Unicode String
	//typedef	wchar_t HSElem;
	//#define	HSElem_Len(s)	wcslen(s)

	typedef	char	HSElem;
#define	HSElem_Len(s)	CSTRING_LENGTH(s)

	typedef	struct _HString
	{
		HSElem *ch;	//若是非空串,则按串长度分配存储区,否则ch为NULL
		size_t length;
	}HString;

	void StrInit(HString *t);
	Status StrDestroy(HString *s);
	Status StrClear(HString *s);
	Status StrAssign(HString *t, HSElem *chars);
	Status StrCopy(HString *t, HString *s);
	Status StrEmpty(HString *s);
	int StrCompare(HString *s, HString *t);
	size_t StrLength(HString *s);
	//将s1和s2连接后存入新串t
	Status StrConcat(HString *t, HString *s1, HString *s2);
	//算法4.3返回串s位置pos起长度为len的子串
	Status StrSubString(HString *sub, HString *s, size_t pos, size_t len);
	//算法4.5的改进,由定长顺序结构串改进到堆分配结构串
	//若子串不存在,返回-1
	size_t StrIndex(HString *s, HString *t, size_t pos);
	//算法4.6模式匹配KMP算法
	size_t StrIndexKMP(HString *s, HString *t, size_t pos);
	//算法4.7求模式串t的next数组
	void KMP_Next(HString *t, int next[]);
	//算法那4.8求模式串t的next修正数组
	void KMP_NextVal(HString *t, int next[]);
	Status StrReplace(HString *s, HString *t, HString *v);
	//算法4.4
	Status StrInsert(HString *s, size_t pos, HString *t);
	Status StrDelete(HString *s, size_t pos, size_t len);

	//补充功能
	//将串s连接到t之后
	Status StrCat(HString *t, HString *s);
	//将字符c连接到t之后
	Status StrCatc(HString *t, HSElem c);
	//返回HSElem指针
	HSElem *StrPointer(HString *t);
	//返回pos位置的字符,若位置非法,返回0
	HSElem StrElem(HString *t, size_t pos);

#ifdef __cplusplus
}
#endif // __cplusplus

