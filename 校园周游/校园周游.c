#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INFINITY 10000
#define M 20                  //最大顶点数

/*景点信息结构体*/
typedef struct {
	int num;                  //景点代号
	char name[20];            //景点名称
	char intro[200];          //景点简介
}vertextype;

typedef int edgtype;          //权值类型

/*校园景点图结构体*/
typedef struct {
	vertextype vexs[M];       //顶点信息域
	edgtype edge[M][M];		  //邻接矩阵
	int vexNum, edgNum;       //顶点数和边数
}mgraphtype;

/*校园景点图的读取与创建*/
void Create_Map(mgraphtype* g)
{
	int i, j, k, e;
	FILE* rf;
	rf = fopen("map.txt", "r");
	if (rf)
	{
		fscanf(rf, "%d%d", &g->vexNum, &g->edgNum);//从文件中读取顶点数和边数
		for (i = 0; i < g->vexNum; i++)
			fscanf(rf, "%d%s%s", &g->vexs[i].num, g->vexs[i].name, g->vexs[i].intro);//从文件中读取每个景点的信息：代号、名称、简介
		for (i = 0; i < g->vexNum; i++)
		{
			for (j = 0; j < g->vexNum; j++)
			{
				if (i == j)
					g->edge[i][j] = 0;
				else
					g->edge[i][j] = INFINITY;
			}
		}
		for (k = 0; k < g->edgNum; k++)
		{
			fscanf(rf, "%d%d%d", &i, &j, &e);//从文件中读取两点间的路径长度
			g->edge[i][j] = g->edge[j][i] = e;
		}
		fclose(rf);
	}
	else
		g->edgNum = 0;
}

/*校园景点图的显示*/
void Dis_Map() 
{
	printf("\n");	
	printf("—————————『杭电校园景点地图一览』——————————		\n\n");
	printf("                    （1）杭电大门									\n");
	printf("                             @										\n");
	printf("	                     |												\n");
	printf("       （2）月牙湖 @-—————————@ （3）行政楼					\n");
	printf("	           |         |         |									\n");
	printf("	           |         |         |									\n");
	printf("         （4）一教 @-—————————@ （5）二教					\n");
	printf("	           |         |         |									\n");
	printf("	           |         |         |									\n");
	printf("         （6）三教 @-—————————@————————@ （12）排球场\n");
	printf("	           |         |         | （7）四教      |				\n");
	printf("	           |         |         |                |				\n");
	printf("	 （8）七教 @-—————————@————————@ （11）体育馆	\n");
	printf("	           |         |         | （9）六教			    		\n");
	printf("	           |         |         |								\n");
	printf("	           |—————————-|								\n");
	printf("	                     |											\n");
	printf("	                     @											\n");
	printf("	               （10）图书馆		 								\n");
	printf("	\n");
}

/*景点信息查询*/
void Search_Location(mgraphtype* g)
{
	int s;
	do {
		printf("\n请输入你要查找的景点编号：");
		scanf("%d", &s);
	} while (Judge_Input(s));

	printf("\n景点名称：[%s]\n\n", g->vexs[s - 1].name);
	printf("景点简介： %s\n\n", g->vexs[s - 1].intro);
}

/*判断输入的编号是否合理*/
int Judge_Input(int num)
{
	int i = 0;
	if (num == -1)
		return i;
	if (num < 1 || num>12)
	{
		printf("\n输入的编号有误，请输入1-12之间的数字！\n");
		i = 1;
	}
	return i;
}

/*Floyd算法求两景点间的一条最短的路径*/
int dist[M][M];	//距离向量
int path[M][M];	//路径向量
void ShortPath(mgraphtype* g) 
{
	int i, j, k;

	for (i = 0; i < g->vexNum; i++)	//初始化距离向量矩阵与路径向量矩阵
	{
		for (j = 0; j < g->vexNum; j++)
		{
			dist[i][j] = g->edge[i][j];
			if (i != j && dist[i][j] < INFINITY) path[i][j] = i;
			else path[i][j] = -1;	//-1代表当前两点不可达
		}
	}
		
	for (k = 0; k < g->vexNum; k++)	//递推求每两个景点间的最短路径
	{
		for (i = 0; i < g->vexNum; i++)
		{
			for (j = 0; j < g->vexNum; j++)	//更新dist[i][j]的值
			{
				if (dist[i][j] > (dist[i][k] + dist[k][j]))
				{
					dist[i][j] = dist[i][k] + dist[k][j];
					path[i][j] = k;	//path用于记录最短路径上的经结点
				}
			}
				
		}
			
	}
}

/*递归实现打印两点间的最短路径*/
void Floyd_Print(mgraphtype* g, int sNum, int eNum) 
{
	if (path[sNum][eNum] == -1 || path[sNum][eNum] == eNum || path[sNum][eNum] == sNum)
		return;
	else 
	{
		Floyd_Print(g, sNum, path[sNum][eNum]);                 //将中间点作为终点继续打印路径
		printf("%s->", g->vexs[path[sNum][eNum]].name);	//打印中间景点名字
		Floyd_Print(g, path[sNum][eNum], eNum);                 //将中间点作为起点继续打印路径
	}
}

/*输出并打印两点间的最短路径*/
void Shortpath_Print(mgraphtype* g) 
{
	int sNum, eNum;	//起点编号，终点编号

	do {
		printf("\n请输入起点编号：");
		scanf("%d", &sNum);
	} while (Judge_Input(sNum));

	do {
		printf("\n请输入终点编号：");
		scanf("%d", &eNum);
	} while (Judge_Input(eNum));

	printf("\n%s到%s的最短距离是：%d米\n", g->vexs[--sNum].name, g->vexs[--eNum].name, dist[sNum][eNum]);

	printf("\n这是最短游览路线：");
	printf("%s->", g->vexs[sNum].name);		//输出路径上的起点
	Floyd_Print(g, sNum, eNum);						//输出路径上的中间点
	printf("%s\n\n", g->vexs[eNum].name);	//输出路径上的终点
}

/*深度优先遍历查询任意两个景点之间的所有路径*/
int pathStack[M];	//路径栈，存储路径信息
int top;			//栈顶
int visited[M];		//入栈标记，防止形成回路
int count=0;		//路径计数器
void Dfs_Print(mgraphtype* g, int sNum, int eNum) 
{
	int dis = 0;			//用于记录路径长度
	pathStack[top] = sNum;	//将本趟起点入栈
	top++;
	visited[sNum] = 1;		//将入栈点标记为已入栈

	for (int i = 0; i < g->vexNum; i++) 
	{
		if (g->edge[sNum][i] > 0 && g->edge[sNum][i] != INFINITY && !visited[i]) 
		{
			//表明前一个入栈点与该点可达，且该点未入栈（未被访问
			if (i == eNum)	//如果深度遍历搜到了终点，就输出刚才的路径
			{
				printf("第%d条路:", count++);
				for (int j = 0; j < top; j++) 
				{
					printf("%s->", g->vexs[pathStack[j]].name);
					if (j < top - 1)
						dis = dis + g->edge[pathStack[j]][pathStack[j + 1]];	//统计路径长度
				}
				dis = dis + g->edge[pathStack[top - 1]][eNum];	//最后一条路单独出来，因为enum不能入栈
				printf("%s\n", g->vexs[eNum].name);
				printf("总长度是：%dm\n\n", dis);
			}
			else 
			{
				Dfs_Print(g, i, eNum);	//如果该点不是终点,接着深度搜索
				top--;						//支路全被访问一遍后，顶点出栈
				visited[i] = 0;				//将出栈点标记为已出栈，允许下次访问
			}
		}
	}
}

/*查询任意两个景点之间的所有路径并打印*/
void Allpath_Print(mgraphtype* g) 
{
	int sNum, eNum;
	count = 1;	//路径计数器
	top = 0;	//栈顶
	memset(pathStack, 0, sizeof(pathStack));	//路径栈初始化
	memset(visited, 0, sizeof(visited));		//入栈标记初始化

	do {
		printf("\n请输入起点编号：");
		scanf("%d", &sNum);
	} while (Judge_Input(sNum));

	do {
		printf("\n请输入终点编号：");
		scanf("%d", &eNum);
	} while (Judge_Input(eNum));
	printf("\n");
	Dfs_Print(g, sNum - 1, eNum - 1);
}

/*主菜单*/
int menu() 
{
	int s;
	printf("\t\t ———————————————————————————————————— \n");
	printf("\t\t│                                                                       │\n");
	printf("\t\t│                     欢迎使用杭电校园导游咨询系统                      │\n");
	printf("\t\t│                                                                       │\n");
	printf("\t\t│                                                                       │\n");
	printf("\t\t│      1.杭电校园景点信息查询              2.两景点间最短路径查询       │\n");
	printf("\t\t│                                                                       │\n");
	printf("\t\t│      3.两景点间所有路径查询              4.退出校园导游咨询系统       │\n");
	printf("\t\t│                                                                       │\n");
	printf("\t\t│                                                                       │\n");
	printf("\t\t ———————————————————————————————————— \n\n");
	printf("\t\t请根据你的需求选择操作：");
	scanf_s("%d", &s);
	printf("\n\n");
	return s;
}

/*退出菜单栏*/
void System_Exit(int* quit) 
{
	*quit = 1;
	printf("\t\t               ——————————————————————              \n");
	printf("\t\t              │           欢迎下次使用，谢谢！            │             \n");
	printf("\t\t               ——————————————————————              \n");
	printf("\t\t                 *      *       * * * *        *       *                 \n");
	printf("\t\t                 *      *       *       *      *       *                 \n");
	printf("\t\t                 * **** *       *       *      *       *                 \n");
	printf("\t\t                 *      *       *       *      *       *                 \n");
	printf("\t\t                 *      *       * * * *          * * *                   \n");
}

/*主函数*/
void main() 
{
	int quit = 0;
	mgraphtype g;

	Create_Map(&g);		//从文件读取信息建立图
	ShortPath(&g);		//Floyd求出dist与path

	while (!quit) //系统退出条件满足判定
	{
		switch (menu()) //打印主菜单
		{
		case 1:system("cls"); Dis_Map(); Search_Location(&g); break;         //校园景点信息查询
		case 2:system("cls"); Dis_Map(); Shortpath_Print(&g); break;         //两景点间最短路径查询
		case 3:system("cls"); Dis_Map(); Allpath_Print(&g); break;           //两景点间所有路径查询
		case 4:system("cls"); System_Exit(&quit); break;                     //退出校园导游咨询系统
		default:printf("\t\t\t\t\t输入错误！没有该选项对应的操作。\n\n");
		}
		system("pause");
		system("cls"); //清屏                                                          
	}
}