#pragma once

#include "DataStructureC.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	LIST_INIT_SIZE	100
#define	LIST_INCREMENT	10

	//����˳���
	typedef	struct _SQLIST
	{
		ELEMENT *elem;					//�洢�ռ��ַ
		size_t length;						//��ǰ����
		size_t listsize;					//��ǰ����Ĵ洢����
		PCOMPAREROUTINE compareroutine;		//�ȽϺ���
		PALLOCATEROUTINE allocateroutine;	//�ڴ���亯��
		PFREEROUTINE freeroutine;			//�ڴ��ͷź���
	}SQLIST, *PSQLIST;

	//�㷨2.3
	Status SqlistInit(SQLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	//�㷨2.4
	Status SqlistInsert(SQLIST *list, size_t pos, ELEMENT *elem);
	//�������
	Status SqlistOrderInsert(SQLIST *list, ELEMENT *elem);
	void SqlistDestroy(SQLIST *list);
	void SqlistClear(SQLIST *list);
	Status SqlistEmpty(SQLIST *list);
	size_t SqlistLength(SQLIST *list);
	Status SqlistGetElem(SQLIST *list, size_t pos, ELEMENT *elem);
	//�㷨2.6
	size_t SqlistLocate(SQLIST *list, ELEMENT *elem);
	//Ϊʵ������˳������ӵĺ������ش���elem�ĵ�һ��Ԫ�ص�λ��
	//������ȵ�Ԫ�ش���,��b�з��ظ�Ԫ��
	size_t SqlistOrderLocate(SQLIST *list, ELEMENT *elem, ELEMENT **b);
	Status SqlistPriorElem(SQLIST *list, ELEMENT *elem, ELEMENT *pre_elem);
	Status SqlistNextElem(SQLIST *list, ELEMENT *elem, ELEMENT *pre_elem);
	//�㷨2.5
	Status SqlistDelete(SQLIST *list, size_t pos, ELEMENT *elem);
	Status SqlistTraverse(SQLIST *list, Status(_cdecl *visit)(ELEMENT *elem));
	//�㷨2.1
	void SqlistUnio(SQLIST *list1, SQLIST *list2);
	//�㷨2.7
	Status SqlistMerge(SQLIST *la, SQLIST *lb, SQLIST *lc);

	//��������
	typedef	PLINKNODE	LINKLISTPOSITION;

	typedef struct _LINKLIST
	{
		LINKNODE *header, *tail;		//�ֱ�ָ��ͷβ�ڵ�
		size_t len;							//Ԫ�ظ���
		PCOMPAREROUTINE compareroutine;		//�ȽϺ���
		PALLOCATEROUTINE allocateroutine;	//�ڴ���亯��
		PFREEROUTINE freeroutine;			//�ڴ��ͷź���
	}LINKLIST,*PLINKLIST;

	Status LinklistMakeNode(PALLOCATEROUTINE allocateroutine, PLINKNODE *p, ELEMENT *elem);
	Status LinklistFreeNode(PFREEROUTINE freeroutine, PLINKNODE *p);
	Status LinklistInit(LINKLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	void LinklistClear(LINKLIST *list);
	void LinklistDestroy(LINKLIST *list);

	//����ڵ������һ�麯��
	//���������ĺͽ̲��㷨2.20���,���º����ƺ�ֻ������ڵ�������ɾ������,����
	//ʵ�ʷ�����ͷŽڵ��ڴ�
	//LinklistInsFirst,LinklistDelFirst������������ƺܲ���,����ֻ��ͷ�ڵ�,û��
	//����ṹ,�������������ڲ����ܶ�����ĳ��ȺͿ��ܸı��βָ���������,��˵���
	//�߱��������޸�������(list->len),���������������Ƿ�Ϊ0,��Ϊ0,Ӧ��������
	//βָ��
	Status LinklistInsFirst(LINKNODE *h, PLINKNODE s);
	Status LinklistDelFirst(LINKNODE *h, PLINKNODE *q);
	Status LinklistAppend(LINKLIST *list, PLINKNODE s);
	Status LinklistRemove(LINKLIST *list, PLINKNODE *q);
	Status LinklistInsBefor(LINKLIST *list, PLINKNODE *p, PLINKNODE s);
	Status LinklistInsAfter(LINKLIST *list, PLINKNODE *p, PLINKNODE s);
	Status LinklistDel(LINKLIST *list, PLINKNODE p, PLINKNODE *q);

	//��Sqlist���Ƶ�һ�����,�Խڵ�Ԫ��ʵ�ʷ�����ͷŽڵ��ڴ�,�ٵ�������һ�麯�����ڵ�Ĳ���ɾ������
	//�㷨2.9
	Status LinklistInsert(LINKLIST *list, size_t pos, ELEMENT *elem);
	//�㷨2.10
	Status LinklistDelete(LINKLIST *list, size_t pos, ELEMENT *elem);
	Status LinklistDeletePos(LINKLIST *list, LINKLISTPOSITION pos);

	Status LinklistSetCurElem(PLINKNODE *p, ELEMENT *elem);
	//�㷨2.8����
	ELEMENT *LinklistGetCurElem(PLINKNODE p);
	Status LinklistEmpty(LINKLIST *list);
	size_t LinklistLength(LINKLIST *list);
	LINKLISTPOSITION LinklistHeader(LINKLIST *list);
	LINKLISTPOSITION LinklistGetLast(LINKLIST *list);
	LINKLISTPOSITION LinklistPriorPos(LINKLIST *list, PLINKNODE p);
	LINKLISTPOSITION LinklistNextPos(LINKLIST *list, PLINKNODE p);
	Status LinklistLocatePos(LINKLIST *list, size_t i, PLINKNODE *p);
	LINKLISTPOSITION LinklistLocateElem(LINKLIST *list, ELEMENT *elem);
	Status LinklistTraverse(LINKLIST *list, Status(_cdecl *visit)(ELEMENT *elem));
	void LinklistUnio(LINKLIST *list1, LINKLIST *list2);
	//�㷨2.21
	//�����Խ̲��㷨2.21����һЩ�޸�
	//��������LinklistMerge�����ڲ���ʼ��lc(�����ڴ�),����Ҫ������������LinklistDestroy(lc)�����ͷ�
	//�ⲻ����"˭����˭�ͷ�"ԭ��,��˱���Ҫ�����lc���Ѿ�����LinklistInit������
	//ͬʱҲ���ͷ�la��lb��ͷ���,�ɵ�������������
	void LinklistMerge(LINKLIST *la, LINKLIST *lb, LINKLIST *lc);

	//һԪ����ʽ
	typedef	LINKLIST	POLYNOMAIL;
	typedef	PLINKLIST	PPOLYNOMAIL;

	/*һԪ����ʽ��Ԫ����������
	��ͨ�ýڵ�������
	ELEMENT.data����Ϊָ��POLYN_ELEMENT_DATA�ṹ��ָ��
	ELEMENT.size=sizeof(POLYN_ELEMENT_DATA)����*/
	typedef	struct _POLYN_ELEMENT_DATA
	{
		float coef;		//ϵ��
		int expn;		//ָ��
	}POLYN_ELEMENT_DATA,*PPOLYN_ELEMENT_DATA;

	//һԪ����ʽ����Ԫ��ֵ�ȽϺ���
	int __cdecl polyn_compare_routine(ELEMENT *firstelem, ELEMENT *secondelem);

	//�����Խ̲������޸�,û�в���һ��һ���ֹ�����ķ�ʽ��ʼ������ʽ������PolynAddMonos�����
	//�����߿������û��ֹ�������������ϵ�������PolynAddMonos��������ʽ,Ҳ����������ɻ�ͨ��������ʽ
	//��������ʽ����
	//�㷨2.22
	Status PolynCreate(POLYNOMAIL *polynmail, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	void PolynDestroy(POLYNOMAIL *polynmail);
	//��֪һԪ����ʽpolynmail����,items�д��item_count������ʽ,������ʽpolynmail������items�еĵ���ʽ���,
	//�������polynmail
	Status PolynAddMonos(POLYNOMAIL *polynmail, POLYN_ELEMENT_DATA *items, int item_count);
	//�㷨2.23����"˭����˭�ͷ�"��ԭ��,����PolynAdd����������pb,����������������
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
	Status LinklistOrderLocalElem(LINKLIST *list, ELEMENT *elem, LINKLISTPOSITION *q);
	//�̲��жԱ�����������Ϊ����compare()==0ʱqָʾ��һ��ֵΪelem�Ľڵ�λ�ò�����TRUE;
	//����qָʾ��һ����elem�����ж�����compare()>0��Ԫ�ص�ǰ����λ�ò�����FALSE
	//�����ݽ̲��㷨2.22������,���Ǽƻ������Ԫ�ش�����������������Ԫ��,Ӧ����������������������ʱ,qָʾβ�ڵ㲢����FALSE
	Status LinklistOrderInsert(LINKLIST *list, ELEMENT *elem);


#ifdef __cplusplus
}
#endif // __cplusplus
