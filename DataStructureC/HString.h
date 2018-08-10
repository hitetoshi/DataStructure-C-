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
	//对算法4.6的轻微改进不动态分配改用全局数组作为next表,优化对next表访问性能
	size_t StrIndexKMPOpt(HString *s, HString *t, size_t pos);
	//算法4.7求模式串t的next数组
	void KMP_Next(HString *t, size_t next[]);
	//算法那4.8求模式串t的next修正数组
	void KMP_NextVal(HString *t, size_t next[]);
	Status StrReplace(HString *s, HString *t, HString *v);
	//算法4.4
	Status StrInsert(HString *s, size_t pos, HString *t);
	Status StrDelete(HString *s, size_t pos, size_t len);

	//补充功能
	//将串s连接到t之后
	Status StrCat(HString *t, HString *s);
	//将字符c连接到t之后
	Status StrCatc(HString *t, HSElem c);
	//返回HSElem指针(方便C代码使用)
	HSElem *StrPointer(HString *t);
	//返回pos位置的字符,若位置非法,返回0
	HSElem StrElem(HString *t, size_t pos);

	/*****************************************************************************
							关于KMP算法效率的讨论
	理论上普通模式匹配算法的最差时间复杂度会达到O(n*m)而KMP算法保证时间复杂度为O(n+m)
	但在实际应用中会发行KMP算法的执行时间远大于普通算法(差别可达10的数量级)
	对模式匹配算法的时间复杂度都是选取"比较"操作作为基本操作

	考察两种算法中"指针回退"的操作:
	普通算法:
	else
	{
		//指针后退重新开始匹配
		i = i - j + 1;
		j = 0;
	}
	编译器优化后的机器码为:
	mov         ecx,1		//立即寻址
	sub         ecx,esi		//寄存器寻址
	add         eax,ecx		//寄存器寻址		i=i-j+1;
	xor         esi,esi		//寄存器寻址		j=0;

	KMP算法:
	else
	{
		//指针回退
		j = next[j];
	}
	编译器优化后的机器码为:
	mov         eax,dword ptr [edi+eax*4]	//基址变址	j=next[j];

	可见,反而是普通算法中立即寻址和寄存器寻址的速度要快的多,KMP算法必须使用一个内存中
	的next表,只能以基址变址的寻址方式访问这在很大程度上影响了KMP算法的实际效率。
	
	通过此例可见,衡量算法优劣的指标不应仅限于时间复杂度和空间复杂度;还应考虑选择正确的
	基本操作,教材中选择"比较"作为基本操作,本质上是两个串的"内存比较"。在内存比较操作中,
	影响效率的关键因素应是"内存访问"而非"比较"(cmp指令的开销远低于基址变址寻址的开销),
	虽然KMP算法的时间复杂度为O(n+m)但考虑每次失配都滑动模式串,在最坏的情况下,KMP算法
	的基本操作频度为2n+m。

	根据以上分析,本例对教材算法4.6做了一个极小的修改:不使用堆分配动态创建next数组,而采用
	全局变量size_t g_next[1024],运行可以观察KMP算法效率有了很大提升,这时因为对全局数组
	的访问使用相对寻址,速度比基址变址更快。
	*****************************************************************************/

#ifdef __cplusplus
}
#endif // __cplusplus

