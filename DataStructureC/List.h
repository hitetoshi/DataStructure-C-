#pragma once


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	LIST_INIT_SIZE	100
#define	LIST_INCREMENT	10

	//线性顺序表
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
	Status SqlistTraverse(SQLIST *list, Status(_cdecl *visit)(NODEELEMENT *elem));
	void SqlistUnio(SQLIST *list1, SQLIST *list2);


	//线性链表
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
	Status LinklistDel(LINKLIST *list, PLINKLISTNODE p, PLINKLISTNODE *q);

	//与Sqlist类似的一组操作,对节点元素实际分配和释放节点内存,再调用上面一组函数做节点的插入删除操作
	Status LinklistInsert(LINKLIST *list, size_t pos, NODEELEMENT *elem);
	Status LinklistDelete(LINKLIST *list, size_t pos, NODEELEMENT *elem);
	Status LinklistDeletePos(LINKLIST *list, LINKLISTPOSITION pos);

	Status LinklistSetCurElem(PLINKLISTNODE *p, NODEELEMENT *elem);
	NODEELEMENT *LinklistGetCurElem(PLINKLISTNODE p);
	Status LinklistEmpty(LINKLIST *list);
	size_t LinklistLength(LINKLIST *list);
	LINKLISTPOSITION LinklistHeader(LINKLIST *list);
	LINKLISTPOSITION LinklistGetLast(LINKLIST *list);
	LINKLISTPOSITION LinklistPriorPos(LINKLIST *list, PLINKLISTNODE p);
	LINKLISTPOSITION LinklistNextPos(LINKLIST *list, PLINKLISTNODE p);
	Status LinklistLocatePos(LINKLIST *list, size_t i, PLINKLISTNODE *p);
	LINKLISTPOSITION LinklistLocateElem(LINKLIST *list, NODEELEMENT *elem);
	Status LinklistTraverse(LINKLIST *list, Status(_cdecl *visit)(NODEELEMENT *elem));
	void LinklistUnio(LINKLIST *list1, LINKLIST *list2);

	//一元多项式
	typedef	LINKLIST	POLYNOMAIL;
	typedef	PLINKLIST	PPOLYNOMAIL;

	/*一元多项式的元素数据类型
	在通用节点类型中
	NODEELEMENT.data设置为指向POLYN_ELEMENT_DATA结构的指针
	NODEELEMENT.size=sizeof(POLYN_ELEMENT_DATA)即可*/
	typedef	struct _POLYN_ELEMENT_DATA
	{
		float coef;		//系数
		int expn;		//指数
	}POLYN_ELEMENT_DATA,*PPOLYN_ELEMENT_DATA;

	//一元多项式链表元素值比较函数
	int __cdecl polyn_compare_routine(NODEELEMENT *firstelem, NODEELEMENT *secondelem);

	Status PolynCreate(POLYNOMAIL *polynmail, int m, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	void PolynPrint(POLYNOMAIL *polynmail);

	//教材特别说明的有序链表与基本链表不同职能的LocalElem和增加的OrderInsert
	Status LinklistOrderLocalElem(LINKLIST *list, NODEELEMENT *elem, LINKLISTPOSITION *q);
	//教材中对本操作的描述为满足compare()==0时q指示第一个值为elem的节点位置并返回TRUE;
	//否则q指示第一个与elem满足判定函数compare()>0的元素的前驱的位置并返回FALSE
	//但根据教材算法2.22的描述,考虑计划插入的元素大于有序链表中所有元素,应补充若上述条件都不满足时,q指示尾节点并返回FALSE
	Status LinklistOrderInsert(LINKLIST *list, NODEELEMENT *elem);


#ifdef __cplusplus
}
#endif // __cplusplus
