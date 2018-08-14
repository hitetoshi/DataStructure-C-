#pragma once

#include "DataStructureC.h"
#include "List.h"

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

	typedef	struct _HSTRING
	{
		HSElem *ch;	//教材:若是非空串,则按串长度分配存储区,否则ch为NULL
					//本例:若是空串,则ch[0]==0, length==0
		size_t length;
	}HSTRING;

	void StrInit(HSTRING *t);
	Status StrDestroy(HSTRING *s);
	Status StrClear(HSTRING *s);
	Status StrAssign(HSTRING *t, HSElem *chars);
	Status StrCopy(HSTRING *t, HSTRING *s);
	Status StrEmpty(HSTRING *s);
	int StrCompare(HSTRING *s, HSTRING *t);
	size_t StrLength(HSTRING *s);
	//将s1和s2连接后存入新串t
	Status StrConcat(HSTRING *t, HSTRING *s1, HSTRING *s2);
	//算法4.3返回串s位置pos起长度为len的子串
	Status StrSubString(HSTRING *sub, HSTRING *s, size_t pos, size_t len);
	//算法4.5的改进,由定长顺序结构串改进到堆分配结构串
	//若子串不存在,返回-1
	size_t StrIndex(HSTRING *s, HSTRING *t, size_t pos);
	//算法4.6模式匹配KMP算法
	size_t StrIndexKMP(HSTRING *s, HSTRING *t, size_t pos);
	//对算法4.6的轻微改进不动态分配改用全局数组作为next表,优化对next表访问性能
	size_t StrIndexKMPOpt(HSTRING *s, HSTRING *t, size_t pos);
	//算法4.7求模式串t的next数组
	void KMP_Next(HSTRING *t, size_t next[]);
	//算法那4.8求模式串t的next修正数组
	void KMP_NextVal(HSTRING *t, size_t next[]);
	//用v替换s中所有与t相等的不重叠的字串
	Status StrReplace(HSTRING *s, HSTRING *t, HSTRING *v);
	//算法4.4
	//在串第pos个字符之前插入串t
	Status StrInsert(HSTRING *s, size_t pos, HSTRING *t);
	//从串s中删除pos个字符起长度为len的子串
	Status StrDelete(HSTRING *s, size_t pos, size_t len);

	//补充功能
	//将串s连接到t之后
	Status StrCat(HSTRING *t, HSTRING *s);
	//将字符c连接到t之后
	Status StrCatc(HSTRING *t, HSElem c);
	//返回HSElem指针(方便C代码使用)
	HSElem *StrPointer(HSTRING *t);
	//返回pos位置的字符,若位置非法,返回0
	HSElem StrElem(HSTRING *t, size_t pos);

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

	根据以上分析,本例对教材算法4.6做了一个极小的修改:不使用堆分配动态创建next数组,而采用
	全局变量size_t g_next[1024],运行可以观察KMP算法效率有了很大提升,这时因为对全局数组
	的访问使用相对寻址,速度比基址变址更快。
	*****************************************************************************/


	//关键词索引表

/*教材定义的各项常数,基本上用不上,因为本例使用的堆分配结构串、顺序表、链表都
可以动态分配内存,不需要设定最大长度

#define	MAX_BOOK_NUM	1000	//假设只对1000本书建索引表
#define	MAX_KEY_NUM		2500	//索引表最大容量
#define	MAX_LIN_LEN		500		//书目串的最大长度
#define	MAX_WORD_NUM	10		//词表的最大容量
*/

	/*教材上对词表数据类型的定义
	本例没有采用这一的定义,而是将词表的数据类型定义为HString:
	一是为了本节内容活学活用
	二是顺序表的元素类型本来就为void *,只需要在本例中将其解释为HString *即可,无需另外定义
	根据教材定义,词表只能是定长表
	typedef	struct
	{
		char *item[];
		int last;
	}WordListType;*/

	//将书号链表类型定义为int,没有必要定义,只需要在本应用中将
	//链表ELEMENT.data解释为int *即可
	//typedef	int ElemType;

	//索引项类型
	typedef	struct
	{
		HSTRING key;		//关键词
		LINKLIST bnolist;	//存放书号索引的链表
	}IDXTERMTYPE;

	/*教材上对索引表类型的定义
	本例没有采用这一定义,使用SQLIST作为索引表结构
	将ELEMENT.data类型解释为IDXTERMTYPE *
	因本例采用的顺序表为动态表,所以没有表长限制
	typedef	struct
	{
		IDXTERMTYPE item[MAX_KEY_NUM];
		int last;
	}IdxListType;*/

	//创建索引表,相当于教材算法4.9main函数idxlist为线性顺序表
	Status CreateIdxList(SQLIST *idxlist);
	void DestroyIdxList(SQLIST *idxlist);

	//初始化操作,置索引表idxlist为空表
	//注意:与教材不同,利用已经封装好的线性表结构,没有必要将idxlist.item[0]设为一空串
	Status InitIdxList(SQLIST *idxlist);
	//算法4.10将书号为bno的书名关键词按词典顺序插入索引表idxlist
	Status InsertIdxList(SQLIST *idxlist, int bno, SQLIST *wdlist);
	//算法4.11用wd返回词表wdlist中第i个关键词
	Status GetWord(SQLIST *wdlist, size_t i, HSTRING **wd);

	//算法4.13在索引表idxlist第i项上插入新关键词wd并初始化书号索引的链表为空表
	//因为多处内存分配操作,还是修改返回值类型为Status为宜
	Status InsertNewKey(SQLIST *idxlist, size_t i, HSTRING *wd);
	//算法4.14在索引表idxlist的第i项中插入书号为bno的索引
	Status InsertBook(SQLIST *idxlist, size_t i, int bno);

	//索引表的比较函数
	//算法4.12使用Locate函数遍历索引表查找关键词是否存在
	//本例在索引表初始化时,使用IdxListCompare作为compare函数
	//则可以使用线性表的SqlistOrderLocate操作来查询关键词
	int __cdecl IdxListCompare(ELEMENT *first, ELEMENT *second);

	/*****************************************************************************
								关键词索引表的讨论
	
	通过此例我们体会到,先前我们数据结构的数据类型定义为抽象的void *并且在结构的初
	始化操作中增加了Compare函数,使得我们使用这些数据结构的时候不需要再重新定义数据
	类型,针对特定数据类型的操作(查找、定位、数据移动等)都不需要重写代码。这大大减
	轻了我们代码的编写量,增加了数据结构重用的效率。
	*****************************************************************************/
	

#ifdef __cplusplus
}
#endif // __cplusplus

