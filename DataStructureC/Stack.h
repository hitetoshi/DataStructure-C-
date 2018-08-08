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
	//遍历栈
	//此函数对教材定义作了扩充,增加了一个可由调用者定义的参数param,
	//StackTraverse将此参数传给回调函数visit,方便在类似迷宫求解问题中
	//遍历栈查找给定元素是否存在的操作,可参考PrintMaze函数对它的用法
	Status StackTraverse(SQSTACK *stack, Status(*visit)(ELEMENTNODE *e, void *param), void *param);

	//算法3.1
	//输入十进制数num,转为base进制数并打印
	void conversion(int num, int base);

	//括号匹配,输入字符串ch,检查匹配是否成功并打印
	//可以匹配的括号有:()[]{}
	void brackets(char *ch);

	//算法3.2
	//接收终端输入,并将最终输入存入用户数据缓冲区buf,buf的大小由buf_size指定
	//输入EOF结束行编辑,输入"#"表示退格,"@"表示退行
	//成功返回OK,若用户数据缓冲区长度不足,返回BUFFER_TOO_SMALL
	Status LineEdit(char *buf, size_t buf_size);

	//迷宫栈元素类型
	typedef	struct _MAZEELEMENT
	{
		int ord;	//通道块在路径上的序号
		COORDINATE seat;	//通道块在迷宫中的坐标位置
		int di;		//从此通道块走向下一通道块的方向
	}MAZEELEMENT;

	//算法3.3迷宫求解
	//迷宫图由二维数组point表示,x为迷宫宽度,y为迷宫高度
	//point[xn][yn]==0表示通道,point[xn][yn]==1表示墙壁
	//start为起始位置,end为终点
	//若迷宫求解成功,返回TRUE,路径存入栈path
	//若迷宫求解失败,返回FALSE
	Status MazePath(int **point, int x, int y, COORDINATE *start, COORDINATE *end, SQSTACK *path);

	//算法3.4表达式求值
	//本例对教材算法3.4稍作修改:
	//1.不在函数内部接收控制台输入,而是接收一个以‘#’为结束符的表达式字符串expression(可方便主调函数传入表达式)
	//2.计算结果以result返回给主调函数
	//3.对操作数的处理稍作修改,支持多位数的表达式
	//4.计算成功函数返回OK,否则返回ERROR
	Status EvaluateExpression(char *expression, int *result);


#ifdef __cplusplus
}
#endif // __cplusplus