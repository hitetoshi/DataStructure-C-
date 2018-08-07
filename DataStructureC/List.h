#pragma once

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	LIST_INIT_SIZE	100
#define	LIST_INCREMENT	10

	//����˳���
	typedef	struct _SQLIST
	{
		ELEMENTNODE *elem;					//�洢�ռ��ַ
		size_t length;						//��ǰ����
		size_t listsize;					//��ǰ����Ĵ洢����
		PCOMPAREROUTINE compareroutine;		//�ȽϺ���
		PALLOCATEROUTINE allocateroutine;	//�ڴ���亯��
		PFREEROUTINE freeroutine;			//�ڴ��ͷź���
	}SQLIST, *PSQLIST;

	Status SqlistInit(SQLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	Status SqlistInsert(SQLIST *list, size_t pos, ELEMENTNODE *elem);
	//�������
	Status SqlistOrderInsert(SQLIST *list, ELEMENTNODE *elem);
	void SqlistDestroy(SQLIST *list);
	void SqlistClear(SQLIST *list);
	Status SqlistEmpty(SQLIST *list);
	size_t SqlistLength(SQLIST *list);
	Status SqlistGetElem(SQLIST *list, size_t pos, ELEMENTNODE *elem);
	size_t SqlistLocate(SQLIST *list, ELEMENTNODE *elem);
	//Ϊʵ������˳������ӵĺ������ش���elem�ĵ�һ��Ԫ�ص�λ��
	size_t SqlistOrderLocate(SQLIST *list, ELEMENTNODE *elem);
	Status SqlistPriorElem(SQLIST *list, ELEMENTNODE *elem, ELEMENTNODE *pre_elem);
	Status SqlistNextElem(SQLIST *list, ELEMENTNODE *elem, ELEMENTNODE *pre_elem);
	Status SqlistDelete(SQLIST *list, size_t pos, ELEMENTNODE *elem);
	Status SqlistTraverse(SQLIST *list, Status(_cdecl *visit)(ELEMENTNODE *elem));
	void SqlistUnio(SQLIST *list1, SQLIST *list2);
	Status SqlistMerge(SQLIST *la, SQLIST *lb, SQLIST *lc);

	//��������
	typedef	struct _LINKLIST_NODE
	{
		ELEMENTNODE elem;
		struct _LINKLIST_NODE *next;
	}LINKLISTNODE, *PLINKLISTNODE, *LINKLISTPOSITION;

	typedef struct _LINKLIST
	{
		LINKLISTNODE *header, *tail;		//�ֱ�ָ��ͷβ�ڵ�
		size_t len;							//Ԫ�ظ���
		PCOMPAREROUTINE compareroutine;		//�ȽϺ���
		PALLOCATEROUTINE allocateroutine;	//�ڴ���亯��
		PFREEROUTINE freeroutine;			//�ڴ��ͷź���
	}LINKLIST,*PLINKLIST;

	Status LinklistMakeNode(PALLOCATEROUTINE allocateroutine, PLINKLISTNODE *p, ELEMENTNODE *elem);
	Status LinklistFreeNode(PFREEROUTINE freeroutine, PLINKLISTNODE *p);
	Status LinklistInit(LINKLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	void LinklistClear(LINKLIST *list);
	void LinklistDestroy(LINKLIST *list);

	//����ڵ������һ�麯��
	//���������ĺͽ̲��㷨2.20���,���º����ƺ�ֻ������ڵ�������ɾ������,����
	//ʵ�ʷ�����ͷŽڵ��ڴ�
	Status LinklistInsFirst(LINKLISTNODE *h, PLINKLISTNODE s);
	Status LinklistDelFirst(LINKLISTNODE *h, PLINKLISTNODE *q);
	Status LinklistAppend(LINKLIST *list, PLINKLISTNODE s);
	Status LinklistRemove(LINKLIST *list, PLINKLISTNODE *q);
	Status LinklistInsBefor(LINKLIST *list, PLINKLISTNODE *p, PLINKLISTNODE s);
	Status LinklistInsAfter(LINKLIST *list, PLINKLISTNODE *p, PLINKLISTNODE s);
	Status LinklistDel(LINKLIST *list, PLINKLISTNODE p, PLINKLISTNODE *q);

	//��Sqlist���Ƶ�һ�����,�Խڵ�Ԫ��ʵ�ʷ�����ͷŽڵ��ڴ�,�ٵ�������һ�麯�����ڵ�Ĳ���ɾ������
	Status LinklistInsert(LINKLIST *list, size_t pos, ELEMENTNODE *elem);
	Status LinklistDelete(LINKLIST *list, size_t pos, ELEMENTNODE *elem);
	Status LinklistDeletePos(LINKLIST *list, LINKLISTPOSITION pos);

	Status LinklistSetCurElem(PLINKLISTNODE *p, ELEMENTNODE *elem);
	ELEMENTNODE *LinklistGetCurElem(PLINKLISTNODE p);
	Status LinklistEmpty(LINKLIST *list);
	size_t LinklistLength(LINKLIST *list);
	LINKLISTPOSITION LinklistHeader(LINKLIST *list);
	LINKLISTPOSITION LinklistGetLast(LINKLIST *list);
	LINKLISTPOSITION LinklistPriorPos(LINKLIST *list, PLINKLISTNODE p);
	LINKLISTPOSITION LinklistNextPos(LINKLIST *list, PLINKLISTNODE p);
	Status LinklistLocatePos(LINKLIST *list, size_t i, PLINKLISTNODE *p);
	LINKLISTPOSITION LinklistLocateElem(LINKLIST *list, ELEMENTNODE *elem);
	Status LinklistTraverse(LINKLIST *list, Status(_cdecl *visit)(ELEMENTNODE *elem));
	void LinklistUnio(LINKLIST *list1, LINKLIST *list2);
	//�����Խ̲��㷨2.21����һЩ�޸�
	//��������LinklistMerge�����ڲ���ʼ��lc(�����ڴ�),����Ҫ������������LinklistDestroy(lc)�����ͷ�
	//�ⲻ����"˭����˭�ͷ�"ԭ��,��˱���Ҫ�����lc���Ѿ�����LinklistInit������
	//ͬʱҲ���ͷ�la��lb��ͷ���,�ɵ��ú�����������
	void LinklistMerge(LINKLIST *la, LINKLIST *lb, LINKLIST *lc);

	//һԪ����ʽ
	typedef	LINKLIST	POLYNOMAIL;
	typedef	PLINKLIST	PPOLYNOMAIL;

	/*һԪ����ʽ��Ԫ����������
	��ͨ�ýڵ�������
	ELEMENTNODE.data����Ϊָ��POLYN_ELEMENT_DATA�ṹ��ָ��
	ELEMENTNODE.size=sizeof(POLYN_ELEMENT_DATA)����*/
	typedef	struct _POLYN_ELEMENT_DATA
	{
		float coef;		//ϵ��
		int expn;		//ָ��
	}POLYN_ELEMENT_DATA,*PPOLYN_ELEMENT_DATA;

	//һԪ����ʽ����Ԫ��ֵ�ȽϺ���
	int __cdecl polyn_compare_routine(ELEMENTNODE *firstelem, ELEMENTNODE *secondelem);

	//�����Խ̲������޸�,û�в���һ��һ���ֹ�����ķ�ʽ��ʼ������ʽ������PolynAddMonos�����
	//�����߿������û��ֹ�������������ϵ�������PolynAddMonos��������ʽ,Ҳ����������ɻ�ͨ��������ʽ
	//��������ʽ����
	Status PolynCreate(POLYNOMAIL *polynmail, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	void PolynDestroy(POLYNOMAIL *polynmail);
	//��֪һԪ����ʽpolynmail����,items�д��item_count������ʽ,������ʽpolynmail������items�еĵ���ʽ���,
	//�������polynmail
	Status PolynAddMonos(POLYNOMAIL *polynmail, POLYN_ELEMENT_DATA *items, int item_count);
	//����"˭����˭�ͷ�"��ԭ��,����PolynAdd����������pb,����������������
	void PolynAdd(POLYNOMAIL *pa, POLYNOMAIL *pb);
	void PolynSubtract(POLYNOMAIL *pa, POLYNOMAIL *pb);
	//һԪ����ʽp����,��p���Գ���constant
	void PolynMultiConstant(POLYNOMAIL *p, float constant);
	//һԪ����ʽp����,��p���Ե���ʽmono
	void PolynMultiMono(POLYNOMAIL *p, POLYN_ELEMENT_DATA *mono);
	//һԪ����ʽpa��pb����,��pa��pb���,��������pc
	void PolynMulti(POLYNOMAIL *pa, POLYNOMAIL *pb, POLYNOMAIL *pc);
	void PolynPrint(POLYNOMAIL *polynmail);

	//�̲��ر�˵���������������������ְͬ�ܵ�LocalElem�����ӵ�OrderInsert
	Status LinklistOrderLocalElem(LINKLIST *list, ELEMENTNODE *elem, LINKLISTPOSITION *q);
	//�̲��жԱ�����������Ϊ����compare()==0ʱqָʾ��һ��ֵΪelem�Ľڵ�λ�ò�����TRUE;
	//����qָʾ��һ����elem�����ж�����compare()>0��Ԫ�ص�ǰ����λ�ò�����FALSE
	//�����ݽ̲��㷨2.22������,���Ǽƻ������Ԫ�ش�����������������Ԫ��,Ӧ����������������������ʱ,qָʾβ�ڵ㲢����FALSE
	Status LinklistOrderInsert(LINKLIST *list, ELEMENTNODE *elem);


#ifdef __cplusplus
}
#endif // __cplusplus
