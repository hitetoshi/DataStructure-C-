#pragma once

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	STACK_INIT_SIZE	100
#define	STACKINCREMENT	10

	typedef	struct _SQSTACK
	{
		ELEMENTNODE *base;
		ELEMENTNODE *top;
		int stacksize;
		PALLOCATEROUTINE allocateroutine;
		PFREEROUTINE freeroutine;
	}SQSTACK;

	Status StackInit(SQSTACK *stack, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	Status StackDestroy(SQSTACK *stack);
	Status StackClear(SQSTACK *stack);
	Status StackEmpty(SQSTACK *stack);
	size_t StackLength(SQSTACK *stack);
	Status StackGetTop(SQSTACK *stack, ELEMENTNODE *e);
	Status StackPush(SQSTACK *stack, ELEMENTNODE *e);
	Status StackPop(SQSTACK *stack, ELEMENTNODE *e);
	//����ջ
	//�˺����Խ̲Ķ�����������,������һ�����ɵ����߶���Ĳ���param,
	//StackTraverse���˲��������ص�����visit,�����������Թ����������
	//����ջ���Ҹ���Ԫ���Ƿ���ڵĲ���,�ɲο�PrintMaze�����������÷�
	Status StackTraverse(SQSTACK *stack, Status(*visit)(ELEMENTNODE *e, void *param), void *param);

	//�㷨3.1
	//����ʮ������num,תΪbase����������ӡ
	void conversion(int num, int base);

	//����ƥ��,�����ַ���ch,���ƥ���Ƿ�ɹ�����ӡ
	//����ƥ���������:()[]{}
	void brackets(char *ch);

	//�㷨3.2
	//�����ն�����,����������������û����ݻ�����buf,buf�Ĵ�С��buf_sizeָ��
	//����EOF�����б༭,����"#"��ʾ�˸�,"@"��ʾ����
	//�ɹ�����OK,���û����ݻ��������Ȳ���,����BUFFER_TOO_SMALL
	Status LineEdit(char *buf, size_t buf_size);

	//�Թ�ջԪ������
	typedef	struct _MAZEELEMENT
	{
		int ord;	//ͨ������·���ϵ����
		COORDINATE seat;	//ͨ�������Թ��е�����λ��
		int di;		//�Ӵ�ͨ����������һͨ����ķ���
	}MAZEELEMENT;

	//�㷨3.3�Թ����
	//�Թ�ͼ�ɶ�ά����point��ʾ,xΪ�Թ����,yΪ�Թ��߶�
	//point[xn][yn]==0��ʾͨ��,point[xn][yn]==1��ʾǽ��
	//startΪ��ʼλ��,endΪ�յ�
	//���Թ����ɹ�,����TRUE,·������ջpath
	//���Թ����ʧ��,����FALSE
	Status MazePath(int **point, int x, int y, COORDINATE *start, COORDINATE *end, SQSTACK *path);

	//�㷨3.4���ʽ��ֵ
	//�����Խ̲��㷨3.4�����޸�:
	//1.���ں����ڲ����տ���̨����,���ǽ���һ���ԡ�#��Ϊ�������ı��ʽ�ַ���expression(�ɷ�����������������ʽ)
	//2.��������result���ظ���������
	//3.�Բ������Ĵ��������޸�,֧�ֶ�λ���ı��ʽ
	//4.����ɹ���������OK,���򷵻�ERROR
	Status EvaluateExpression(char *expression, int *result);


#ifdef __cplusplus
}
#endif // __cplusplus