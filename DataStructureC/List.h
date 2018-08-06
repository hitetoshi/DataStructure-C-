#pragma once


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	LIST_INIT_SIZE	100
#define	LIST_INCREMENT	10

	typedef	struct _SQLIST
	{
		NODEELEMENT *elem;					//存储空间基址
		size_t length;						//当前长度
		size_t listsize;					//当前分配的存储容量
		PCOMPAREROUTINE compareroutine;		//比较函数
		PALLOCATEROUTINE allocateroutine;	//内存分配函数
		PFREEROUTINE freeroutine;			//内存释放函数

	}SQLIST, *PSQLIST;

	Status SqlistInit(SQLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	Status SqlistInsert(SQLIST *list, size_t pos, NODEELEMENT *elem);
	void SqlistDestroy(SQLIST *list);
	void SqlistClear(SQLIST *list);
	Status SqlistEmpty(SQLIST *list);
	size_t SqlistLength(SQLIST *list);
	Status SqlistGetElem(SQLIST *list, size_t pos, NODEELEMENT *elem);
	size_t SqlistLocate(SQLIST *list, NODEELEMENT *elem);
	Status SqlistPriorElem(SQLIST *list, NODEELEMENT *elem, NODEELEMENT *pre_elem);
	Status SqlistNextElem(SQLIST *list, NODEELEMENT *elem, NODEELEMENT *pre_elem);
	Status SqlistDelete(SQLIST *list, size_t pos, NODEELEMENT *elem);
	void SqlistUnio(SQLIST *list1, SQLIST *list2);

	typedef	struct _LINKLIST_NODE
	{
		NODEELEMENT elem;
		struct _LINKLIST_NODE *next;
	}LINKLISTNODE, *PLINKLISTNODE, *LINKLISTPOSITION;

	typedef struct _LINKLIST
	{
		LINKLISTNODE *header, *tail;		//分别指向头尾节点
		size_t len;							//元素个数
		PCOMPAREROUTINE compareroutine;		//比较函数
		PALLOCATEROUTINE allocateroutine;	//内存分配函数
		PFREEROUTINE freeroutine;			//内存释放函数
	}LINKLIST,*PLINKLIST;

	Status LinklistMakeNode(PALLOCATEROUTINE allocateroutine, PLINKLISTNODE *p, NODEELEMENT *elem);
	Status LinklistFreeNode(PFREEROUTINE freeroutine, PLINKLISTNODE *p);
	Status LinklistInit(LINKLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	void LinklistClear(LINKLIST *list);
	void LinklistDestroy(LINKLIST *list);

	//链表节点操作的一组函数
	//根据上下文和教材算法2.20理解,以下函数似乎只对链表节点做插入删除操作,并不
	//实际分配和释放节点内存
	Status LinklistInsFirst(LINKLISTNODE *h, PLINKLISTNODE s);
	Status LinklistDelFirst(LINKLISTNODE *h, PLINKLISTNODE *q);
	Status LinklistAppend(LINKLIST *list, PLINKLISTNODE s);
	Status LinklistRemove(LINKLIST *list, PLINKLISTNODE *q);
	Status LinklistInsBefor(LINKLIST *list, PLINKLISTNODE *p, PLINKLISTNODE s);
	Status LinklistInsAfter(LINKLIST *list, PLINKLISTNODE *p, PLINKLISTNODE s);

	//与Sqlist相同的一组操作,对节点元素实际分配和释放节点内存,再调用上面一组函数做节点的插入删除操作
	//根据教材的定义和算法2.20,Linklist必须包含一个没有数据的头结点,这导致Linklist各节点的地位不是平等的。
	//本例按C语言习惯,对有关节点序号的操作排除头结点,即以下函数中pos=0表示访问第一个数据节点
	Status LinklistInsert(LINKLIST *list, size_t pos, NODEELEMENT *elem);
	Status LinklistGetElem(LINKLIST *list, size_t pos, NODEELEMENT *elem);
	size_t LinklistLocate(LINKLIST *list, NODEELEMENT *elem);

	Status LinklistSetCurElem(PLINKLISTNODE *p, NODEELEMENT *elem);
	NODEELEMENT *LinklistGetCurElem(PLINKLISTNODE p);
	Status LinklistEmpty(LINKLIST *list);
	int LinklistLength(LINKLIST *list);
	LINKLISTPOSITION LinklistHeader(LINKLIST *list);
	LINKLISTPOSITION LinklistGetLast(LINKLIST *list);
	LINKLISTPOSITION LinklistPriorPos(LINKLIST *list, PLINKLISTNODE p);
	LINKLISTPOSITION LinklistNextPos(LINKLIST *list, PLINKLISTNODE p);
	Status LinklistLocatePos(LINKLIST *list, size_t i, PLINKLISTNODE *p);
	Status LinklistTraverse(LINKLIST *list, Status(_cdecl *visit)(NODEELEMENT *elem));

#ifdef __cplusplus
}
#endif // __cplusplus
