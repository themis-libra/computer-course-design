#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INFINITY 10000
#define M 20                  //��󶥵���

/*������Ϣ�ṹ��*/
typedef struct {
	int num;                  //�������
	char name[20];            //��������
	char intro[200];          //������
}vertextype;

typedef int edgtype;          //Ȩֵ����

/*У԰����ͼ�ṹ��*/
typedef struct {
	vertextype vexs[M];       //������Ϣ��
	edgtype edge[M][M];		  //�ڽӾ���
	int vexNum, edgNum;       //�������ͱ���
}mgraphtype;

/*У԰����ͼ�Ķ�ȡ�봴��*/
void Create_Map(mgraphtype* g)
{
	int i, j, k, e;
	FILE* rf;
	rf = fopen("map.txt", "r");
	if (rf)
	{
		fscanf(rf, "%d%d", &g->vexNum, &g->edgNum);//���ļ��ж�ȡ�������ͱ���
		for (i = 0; i < g->vexNum; i++)
			fscanf(rf, "%d%s%s", &g->vexs[i].num, g->vexs[i].name, g->vexs[i].intro);//���ļ��ж�ȡÿ���������Ϣ�����š����ơ����
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
			fscanf(rf, "%d%d%d", &i, &j, &e);//���ļ��ж�ȡ������·������
			g->edge[i][j] = g->edge[j][i] = e;
		}
		fclose(rf);
	}
	else
		g->edgNum = 0;
}

/*У԰����ͼ����ʾ*/
void Dis_Map() 
{
	printf("\n");	
	printf("������������������������У԰�����ͼһ������������������������		\n\n");
	printf("                    ��1���������									\n");
	printf("                             @										\n");
	printf("	                     |												\n");
	printf("       ��2�������� @-������������������@ ��3������¥					\n");
	printf("	           |         |         |									\n");
	printf("	           |         |         |									\n");
	printf("         ��4��һ�� @-������������������@ ��5������					\n");
	printf("	           |         |         |									\n");
	printf("	           |         |         |									\n");
	printf("         ��6������ @-������������������@����������������@ ��12������\n");
	printf("	           |         |         | ��7���Ľ�      |				\n");
	printf("	           |         |         |                |				\n");
	printf("	 ��8���߽� @-������������������@����������������@ ��11��������	\n");
	printf("	           |         |         | ��9������			    		\n");
	printf("	           |         |         |								\n");
	printf("	           |������������������-|								\n");
	printf("	                     |											\n");
	printf("	                     @											\n");
	printf("	               ��10��ͼ���		 								\n");
	printf("	\n");
}

/*������Ϣ��ѯ*/
void Search_Location(mgraphtype* g)
{
	int s;
	do {
		printf("\n��������Ҫ���ҵľ����ţ�");
		scanf("%d", &s);
	} while (Judge_Input(s));

	printf("\n�������ƣ�[%s]\n\n", g->vexs[s - 1].name);
	printf("�����飺 %s\n\n", g->vexs[s - 1].intro);
}

/*�ж�����ı���Ƿ����*/
int Judge_Input(int num)
{
	int i = 0;
	if (num == -1)
		return i;
	if (num < 1 || num>12)
	{
		printf("\n����ı������������1-12֮������֣�\n");
		i = 1;
	}
	return i;
}

/*Floyd�㷨����������һ����̵�·��*/
int dist[M][M];	//��������
int path[M][M];	//·������
void ShortPath(mgraphtype* g) 
{
	int i, j, k;

	for (i = 0; i < g->vexNum; i++)	//��ʼ����������������·����������
	{
		for (j = 0; j < g->vexNum; j++)
		{
			dist[i][j] = g->edge[i][j];
			if (i != j && dist[i][j] < INFINITY) path[i][j] = i;
			else path[i][j] = -1;	//-1����ǰ���㲻�ɴ�
		}
	}
		
	for (k = 0; k < g->vexNum; k++)	//������ÿ�������������·��
	{
		for (i = 0; i < g->vexNum; i++)
		{
			for (j = 0; j < g->vexNum; j++)	//����dist[i][j]��ֵ
			{
				if (dist[i][j] > (dist[i][k] + dist[k][j]))
				{
					dist[i][j] = dist[i][k] + dist[k][j];
					path[i][j] = k;	//path���ڼ�¼���·���ϵľ����
				}
			}
				
		}
			
	}
}

/*�ݹ�ʵ�ִ�ӡ���������·��*/
void Floyd_Print(mgraphtype* g, int sNum, int eNum) 
{
	if (path[sNum][eNum] == -1 || path[sNum][eNum] == eNum || path[sNum][eNum] == sNum)
		return;
	else 
	{
		Floyd_Print(g, sNum, path[sNum][eNum]);                 //���м����Ϊ�յ������ӡ·��
		printf("%s->", g->vexs[path[sNum][eNum]].name);	//��ӡ�м侰������
		Floyd_Print(g, path[sNum][eNum], eNum);                 //���м����Ϊ��������ӡ·��
	}
}

/*�������ӡ���������·��*/
void Shortpath_Print(mgraphtype* g) 
{
	int sNum, eNum;	//����ţ��յ���

	do {
		printf("\n����������ţ�");
		scanf("%d", &sNum);
	} while (Judge_Input(sNum));

	do {
		printf("\n�������յ��ţ�");
		scanf("%d", &eNum);
	} while (Judge_Input(eNum));

	printf("\n%s��%s����̾����ǣ�%d��\n", g->vexs[--sNum].name, g->vexs[--eNum].name, dist[sNum][eNum]);

	printf("\n�����������·�ߣ�");
	printf("%s->", g->vexs[sNum].name);		//���·���ϵ����
	Floyd_Print(g, sNum, eNum);						//���·���ϵ��м��
	printf("%s\n\n", g->vexs[eNum].name);	//���·���ϵ��յ�
}

/*������ȱ�����ѯ������������֮�������·��*/
int pathStack[M];	//·��ջ���洢·����Ϣ
int top;			//ջ��
int visited[M];		//��ջ��ǣ���ֹ�γɻ�·
int count=0;		//·��������
void Dfs_Print(mgraphtype* g, int sNum, int eNum) 
{
	int dis = 0;			//���ڼ�¼·������
	pathStack[top] = sNum;	//�����������ջ
	top++;
	visited[sNum] = 1;		//����ջ����Ϊ����ջ

	for (int i = 0; i < g->vexNum; i++) 
	{
		if (g->edge[sNum][i] > 0 && g->edge[sNum][i] != INFINITY && !visited[i]) 
		{
			//����ǰһ����ջ����õ�ɴ�Ҹõ�δ��ջ��δ������
			if (i == eNum)	//�����ȱ����ѵ����յ㣬������ղŵ�·��
			{
				printf("��%d��·:", count++);
				for (int j = 0; j < top; j++) 
				{
					printf("%s->", g->vexs[pathStack[j]].name);
					if (j < top - 1)
						dis = dis + g->edge[pathStack[j]][pathStack[j + 1]];	//ͳ��·������
				}
				dis = dis + g->edge[pathStack[top - 1]][eNum];	//���һ��·������������Ϊenum������ջ
				printf("%s\n", g->vexs[eNum].name);
				printf("�ܳ����ǣ�%dm\n\n", dis);
			}
			else 
			{
				Dfs_Print(g, i, eNum);	//����õ㲻���յ�,�����������
				top--;						//֧·ȫ������һ��󣬶����ջ
				visited[i] = 0;				//����ջ����Ϊ�ѳ�ջ�������´η���
			}
		}
	}
}

/*��ѯ������������֮�������·������ӡ*/
void Allpath_Print(mgraphtype* g) 
{
	int sNum, eNum;
	count = 1;	//·��������
	top = 0;	//ջ��
	memset(pathStack, 0, sizeof(pathStack));	//·��ջ��ʼ��
	memset(visited, 0, sizeof(visited));		//��ջ��ǳ�ʼ��

	do {
		printf("\n����������ţ�");
		scanf("%d", &sNum);
	} while (Judge_Input(sNum));

	do {
		printf("\n�������յ��ţ�");
		scanf("%d", &eNum);
	} while (Judge_Input(eNum));
	printf("\n");
	Dfs_Print(g, sNum - 1, eNum - 1);
}

/*���˵�*/
int menu() 
{
	int s;
	printf("\t\t ������������������������������������������������������������������������ \n");
	printf("\t\t��                                                                       ��\n");
	printf("\t\t��                     ��ӭʹ�ú���У԰������ѯϵͳ                      ��\n");
	printf("\t\t��                                                                       ��\n");
	printf("\t\t��                                                                       ��\n");
	printf("\t\t��      1.����У԰������Ϣ��ѯ              2.����������·����ѯ       ��\n");
	printf("\t\t��                                                                       ��\n");
	printf("\t\t��      3.�����������·����ѯ              4.�˳�У԰������ѯϵͳ       ��\n");
	printf("\t\t��                                                                       ��\n");
	printf("\t\t��                                                                       ��\n");
	printf("\t\t ������������������������������������������������������������������������ \n\n");
	printf("\t\t������������ѡ�������");
	scanf_s("%d", &s);
	printf("\n\n");
	return s;
}

/*�˳��˵���*/
void System_Exit(int* quit) 
{
	*quit = 1;
	printf("\t\t               ��������������������������������������������              \n");
	printf("\t\t              ��           ��ӭ�´�ʹ�ã�лл��            ��             \n");
	printf("\t\t               ��������������������������������������������              \n");
	printf("\t\t                 *      *       * * * *        *       *                 \n");
	printf("\t\t                 *      *       *       *      *       *                 \n");
	printf("\t\t                 * **** *       *       *      *       *                 \n");
	printf("\t\t                 *      *       *       *      *       *                 \n");
	printf("\t\t                 *      *       * * * *          * * *                   \n");
}

/*������*/
void main() 
{
	int quit = 0;
	mgraphtype g;

	Create_Map(&g);		//���ļ���ȡ��Ϣ����ͼ
	ShortPath(&g);		//Floyd���dist��path

	while (!quit) //ϵͳ�˳����������ж�
	{
		switch (menu()) //��ӡ���˵�
		{
		case 1:system("cls"); Dis_Map(); Search_Location(&g); break;         //У԰������Ϣ��ѯ
		case 2:system("cls"); Dis_Map(); Shortpath_Print(&g); break;         //����������·����ѯ
		case 3:system("cls"); Dis_Map(); Allpath_Print(&g); break;           //�����������·����ѯ
		case 4:system("cls"); System_Exit(&quit); break;                     //�˳�У԰������ѯϵͳ
		default:printf("\t\t\t\t\t�������û�и�ѡ���Ӧ�Ĳ�����\n\n");
		}
		system("pause");
		system("cls"); //����                                                          
	}
}