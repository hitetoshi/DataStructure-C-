#pragma once

#include "DataStructureC.h"
#include "List.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	typedef	struct _LINKQUEUE
	{
		PLINKNODE front;
		PLINKNODE rear;
		PALLOCATEROUTINE allocateroutine;
		PFREEROUTINE freeroutine;
		size_t length;
	}LINKQUEUE;

	Status LinkQueueInit(LINKQUEUE *q, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	Status LinkQueueDestroy(LINKQUEUE *q);
	Status LinkQueueClear(LINKQUEUE *q);
	Status LinkQueueEmpty(LINKQUEUE *q);
	size_t LinkQueueLength(LINKQUEUE *q);
	Status LinkQueueGetHead(LINKQUEUE *q, ELEMENT *e);
	Status LinkQueueEn(LINKQUEUE *q, ELEMENT *e);
	Status LinkQueueDe(LINKQUEUE *q, ELEMENT *e);
	Status LinkQueueTraverse(LINKQUEUE *q, Status(_cdecl *visit)(ELEMENT *elem, void *param), void *param);

	//��ɢ�¼�ģ��

#define	MAX_INTERTIME	5	//�������ڵ������еĿͻ���ʱ����������5����
#define	MAX_DURTIME		30	//Ϊÿ���ͻ�����ҵ�񲻳���30����
#define	QUEUE_COUNT		4	//4������

	typedef	struct _BM_EVENT
	{
		int OccurTime;	//�¼�����ʱ��
		int NType;		//�¼�����,0��ʾ�ﵽ�¼�,1-4��ʾ4�����ڵ��뿪�¼�
	}BM_EVENT;

	typedef	LINKLIST	EVENTLIST;

	typedef	struct _BM_CUSTOMER
	{
		int ArrivalTime;	//����ʱ��
		int Duration;		//������������ʱ��
	}BM_CUSTOMER;

	typedef	struct _BANK_SIMULATION_DATA	//ģ�⺯�����õ���ȫ������
	{
		EVENTLIST ev;	//�¼���
		LINKQUEUE q[QUEUE_COUNT];	//4���ͻ�����
		int TotalTime, CustomerNum;	//�ۼƿͻ�����ʱ��,�ͻ���
		int CloseTime;
	}BANK_SIMULATION_DATA;

	//�㷨3.7��ɢ�¼�ģ��
	//�����Խ̲�����һЩ�޸�,����ʹ��ȫ�ֱ��������ȱ��,����:
	//1.���ʹ������ֲ��
	//2.�����������ͻ
	//3.��֧�ֶ��߳�
	//4.ȫ�ֱ������׹���,���������ж������⸳ֵ����������Ų�
	//5.ȫ�ֱ�����������̫��,�Ա���Ϊ��,Bank_Simulation����ִ����Ͼ�Ӧ���ͷ�
	//��ִ�й�����ʹ�õ���������Դ,��Ӧ�ø�ϵͳ��ɶ��⿪��
	//��˱������̲������õ���ȫ��ȫ�ֱ������嵽�ṹ��struct _BANK_SIMULATION_DATA��
	//��������Bank_Simulation�ж���Ϊ�ֲ�����,�Բ�����ʽ���ݸ����������Թ�ʹ��
	//����pEventNotify�ص���������,���¿ͻ��ﵽʱ��ͻ��뿪�¼�����ʱ,Bank_Simulation������pEventNotify֪ͨ��������
	//��������������������Ϣ
	//����TotalTime��CustomerNum,��ģ�����ʱ���ܺ�ʱ���ܿͻ���ͨ���������ظ���������,���������������������Ϣ
	Status Bank_Simulation(int CloseTime, void(__cdecl *pEventNotify)(BM_EVENT *en), int *TotalTime, int *CustomerNum);

	//������ɢ�¼�ģ���㷨������
	//1.�����ȷ����㷨(FCFS)�Ͷ���ҵ�����㷨(SJF)
	//�㷨3.7�������������ݽṹ�����������������¼�,�������洢�ͻ�
	//��ͬһ���ͻ�������,�����е��Ƚ��ȳ�����,�����ܳ���ͬһ�����еȴ�ʱ��϶̵Ŀ��˱ȵȴ�ʱ������Ŀ��˵�ҵ�����ȴ���
	//���ǵ��͵������ȷ����㷨
	//���¼�������,�¼�����ʱ����̵��¼����Ǳ���������ͷ���ȴ���,���ǵ��͵Ķ���ҵ�����㷨

	//2.�Կͻ����еĸĽ�
	//1)�����ȷ����㷨�ǶԳ���ҵ����������ҵ����,�������ڿͻ�����ļ��ʱ���ÿ���ͻ���ƽ��ҵ��ʱ��,����ȷ��FCFS��SJF��
	//���ѡ���ڱ�����,����ͨ���޸�MAX_DURTIME(����ÿλ�ͻ�ҵ������ʱ��),MAX_INTERTIME(���������ͻ��ﵽ���е����ʱ����)
	//��QUEUE_COUNT(���й�̨��)���۲�ƽ��ÿ�û������е�ͣ��ʱ��Ĳ��
	//2)��ʹʹ��FCFS�㷨,�����㷨3.7������4������,��Ȼ���ܳ�����A�������ȴﵽ���еĿͻ���B�����к�ﵽ���еĿͻ���ҵ�����
	//�Ƚ���Ȼ���뷨��ֻʹ��һ���û�����,�������ɱ�֤�����û����������ȷ���
	//�����㷨3.7����ͬ������ʽ,��:ÿ��ֻ���¼��б���ȡ��һ���¼�(���Խ�����ͬ�Ĺ�̨)����,�����¼�������Ϻ���ȡ��һ��
	//�¼�,�ڴ��㷨��,���ֻ����һ���ͻ������൱��ֻ��һ����̨���Դ���ҵ��,�����������Ҫʹ���첽����ʽ(���߳�)
	//�йض��е��첽����,�ǱȽϵ��͵�����������������,��Ҫ��������ϵͳ���ٽ���(CriticalSection)���ź���(Semaphore)��ͬ������
	//����֤���̰߳�ȫ,�ڴ˲����������.

#ifdef __cplusplus
}
#endif // __cplusplus