#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string>
#include <fstream>
#include <iostream>
using namespace std;

#define number 6   //字符个数

/*哈夫曼树存储结构定义*/
typedef struct
{
	int weight;					//权值
	char ch;					//结点对应字符
	int parent, lchild, rchild;	//结点的双亲和左右孩子
}HTNode, * HuffmanTree;			//动态分配数组存储哈夫曼树

typedef char** Huffmancode;//动态分配数组存储哈夫曼编码

/*选择无双亲且权值最小的两个结点*/
void Select(HuffmanTree HT, int n, int& s1, int& s2)
{
	int i = 1, min = 0;

	//先找到第一个双亲为-1的结点的权值暂时作为最小值与其他结点进行比较
	for (i = 1; i <= n; i++)
	{
		if (HT[i].parent == -1)
		{
			min = i;
			break;
		}
	}
	//将上个循环找到的暂时的最小值与每个结点的权值进行比较从而找出权值最小的结点
	for (i = 1; i <= n; i++)
	{
		if (HT[i].parent == -1)
		{
			if (HT[i].weight < HT[min].weight)
			{
				min = i;
			}
		}
	}
	s1 = min;//令s1为权值最小的结点

	//寻找权值次小的结点
	for (i = 1; i <= n; i++)
	{
		if ((HT[i].parent == -1) && (i != s1))
		{
			min = i;
			break;
		}
	}
	
	for (i = 1; i <= n; i++)
	{
		if ((HT[i].parent == -1) && (i != s1))
		{
			if (HT[i].weight < HT[min].weight)
			{
				min = i;
			}
		}
	}
	s2 = min;//令s2为第二个权值最小的结点
}

/*初始化并建立哈夫曼树*/
void CreateTree(HTNode* HT, int n)
{
	int m, i, s1, s2;
	
	//字符均为叶子结点,由二叉树的性质：度为2的结点数+1=叶子节点数可得总结点个数
	m = 2 * n - 1;

	//利用for循环将哈夫曼树中所有结点的权值、双亲和左右孩子赋值为0
	for (i = 1; i < m+1; i++)
	{
		HT[i].ch = '@';
		HT[i].weight = 0;
		HT[i].parent = -1;
		HT[i].lchild = -1;
		HT[i].rchild = -1;
	}

	printf("请输入字符及其权值：\n");

	//利用循环为叶子结点赋权值和字符
	i = 1;
	while (i <= n)
	{
		getchar();

		HT[i].ch = getchar();
		cin >> HT[i].weight;
		i++;
	}

	//建立哈夫曼树
	for (i = n + 1; i < m+1; i++)
	{
		Select(HT, i - 1, s1, s2);//在HT[1...i-1]中选择无双亲且权值最小的两个结点
		HT[s1].parent = i;//s1和s2结合，将i赋给它们的双亲域
		HT[s2].parent = i;
		HT[i].lchild = s1;//将s1与s2赋给i的左孩子域和右孩子域
		HT[i].rchild = s2;
		//i的权值等于左右两个孩子的权值相加
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}

	FILE* fp1;
	fp1 = fopen("hfmTree.txt", "w");
	if (fp1 == NULL)
	{
		printf("文件打开失败\n");
		exit(0);
	}
	//将创建好的哈夫曼树存于文件中
	for (i = 1; i <= m; i++)
	{
		fprintf(fp1, "%c %d %d %d %d\n", HT[i].ch, HT[i].weight,HT[i].parent, HT[i].lchild, HT[i].rchild );
	}
	fclose(fp1);
}

/*要编码的文件*/
void ToBeTran()
{
	char s[50];//存放报文
	printf("请输入要进行编码的报文:\n");
	getchar();
	cin.getline(s, 50);

	FILE* fp;
	fp = fopen("ToBeTran.txt", "w");
	if ( fp == NULL)
	{
		printf("文件打开失败\n");
		exit(0);
	}

	for (int i = 0; s[i] != '\0'; i++)
	{
		fprintf(fp, "%c", s[i]/*Data[i].c*/);
	}
	fprintf(fp, "#");
	fclose(fp);
}

/*编码函数*/
void HuffmanCoding(HTNode* HT, Huffmancode& HC, int n)
{   //HC实际为存储每个编码的头指针数组
	//分配n个字符编码的头指针存储数组空间，0号单元不用
	HC = (Huffmancode)malloc((number + 1) * sizeof(char*));
	int i, start, c, f;
	char* cd;//分配暂时存储编码的工作空间
	cd = (char*)malloc(n * sizeof(char));
	cd[n - 1] = '\0';//编码结束符
	printf("每个字符及其对应编码如下：\n");
	for (i = 1; i <= n; i++)
	{
		//因为是从叶子结点逆序进行编码，所以start指向存储编码的结束符位置
		start = n - 1;
		//从叶子结点到根逆向求编码
		f = HT[i].parent;
		c = i;
		while (f != -1)
		{
			--start;
			if (HT[f].lchild == c)	//如果目前结点是f的左孩子则编码为0
				cd[start] = '0';
			else					//如果目前结点是f的右孩子则编码为1
				cd[start] = '1';

			//接着让c指向目前结点的双亲，f指向目前结点的双亲的双亲不断进行编码
			//直至f指向根结点
			c = f;
			f = HT[f].parent;
		}
		//对每个叶子结点进行编码结束之后便将对应的编码存储到HC中
		//为第i个字符的编码分配空间
		HC[i] = (char*)malloc((number - start) * sizeof(char));
		strcpy(HC[i], &cd[start]);//从cd复制到HC
		printf("%c--%s\t", HT[i].ch, HC[i]);//输出字符及其对应编码
	}
	free(cd);//释放暂时存储编码的工作空间

	cout << endl;
	ToBeTran();//输入报文

	FILE* fp, * fp1;
	fp = fopen("ToBeTran.txt", "r");
	fp1 = fopen("CodeFile.txt", "w");

	//建立CodeFile.txt文件用于存放编码
	if (fp1 == NULL)
	{
		printf("文件打开失败\n");
		exit(0);
	}

	//判断ToBeTran.txt文件是否存在
	if (fp == NULL)
	{
		printf("文件读取失败\n");
		exit(0);
	}
	c = fgetc(fp);
	while (c != '#')
	{
		for (i = 1; i <= n; i++)
		{
			if (c == HT[i].ch) //找到字符编号,将对应的哈夫曼编码写入文件CodeFile.txt中
				fprintf(fp1, "%s", HC[i]);
		}
		c = fgetc(fp);
	}
	fprintf(fp1, "#");
	fclose(fp);
	fclose(fp1);
}

/*译码函数*/
void TranslateTree(HTNode* HT, int n)//从根出发，按字符'0'/'1'来寻找左孩子或者右孩子直至叶子结点
{
	int m;
	char b;

	FILE* fp, * fp1, * fp2;
	fp = fopen("CodeFile.txt", "r");
	fp1 = fopen("TextFile.txt", "w");
	fp2 = fopen("TextFile.txt", "r");

	if (fp == NULL)
	{
		printf("文件打开失败\n");
		exit(0);
	}

	//将翻译过的译码存入TextFile
	if (fp1 == NULL)
	{
		printf("文件打开失败\n");
		exit(0);
	}

	printf("字符串进行译码的结果为：\n");
	b = fgetc(fp); //从CodeFile中读取字符
	m = 2 * n - 1;//初始化让m指向根结点，从根结点出发
	while (b != '#')
	{
		if (b == '0')		//如果为'0'，则指向其左孩子
		{
			m = HT[m].lchild;
		}
		else if (b == '1')	//如果为'1'，则指向其右孩子
		{
			m = HT[m].rchild;
		}
		//一直到根结点则将该结点表示的字符存入文件中并让m重新指向根结点进行查找
		if (HT[m].lchild == -1)
		{
			fprintf(fp1, "%c", HT[m].ch);
			m = 2 * n - 1;
		}
		printf("%c", b);
		b = fgetc(fp);//再读下一个字符
	}
	printf("——");
	fprintf(fp1, "#"); //将‘#’写入文件
	fclose(fp);
	fclose(fp1);

	if (fp2 == NULL)
	{
		printf("文件打开失败\n");
		exit(0);
	}

	char k = fgetc(fp2);
	while (k != '#')
	{
		printf("%c", k);
		k = fgetc(fp2);
	}
	printf("\n\n");
	fclose(fp2);
}

/*打印代码文件*/
void Print() 
{
	FILE* fp1, * fp2;
	fp1 = fopen("CodeFile.txt", "r");
	fp2 = fopen("CodePrin.txt", "w+");

	int count = 0;
	char ch;
	ch = fgetc(fp1);
	while (ch != '#') //文件未达末尾进入循环
	{
		printf("%c", ch);
		fprintf(fp2, "%c", ch);
		count++;
		if (count == 50) 
		{
			printf("\n");
			fprintf(fp2, "\n");
			count = 0;
		}
		ch = fgetc(fp1);
	}
	printf("\n");
	fprintf(fp2, "\n");
	fclose(fp1);
	fclose(fp2);
}

/*打印哈夫曼树*/
void TreePrinting(HTNode* HT,int n,int p)
{
	FILE* fp;
	fp = fopen("TreePrint.txt", "w+");

	int i;
	if (n != -1)
	{
		TreePrinting(HT, HT[n].rchild, p + 1);
		for (i = 0; i < p; i++)
		{
			printf("\t");
			fprintf(fp, "\t");
		}
		printf("%c\n", HT[n].ch);
		fprintf(fp, "%c\n", HT[n].ch);
		TreePrinting(HT, HT[n].lchild, p + 1);
	}
	fclose(fp);
}

void menu()
{
	printf("--------哈夫曼编译码操作系统------\n");
	printf("I.创建哈夫曼树\n");	
	printf("E.编码\n");			
	printf("D.译码\n");			
	printf("P.打印代码文件\n");	
	printf("T.打印哈夫曼树\n");	
	printf("B.退出程序\n");		
	printf("----------------------------------\n");
	printf("输入你的选择：");
}

int main()
{
	int n;
	char a;

	//申请m+1个结点的存储空间，其中0号单位不用
	HTNode* HT = new HTNode[2 * number];
	Huffmancode HC = NULL;

	printf("请输入字符个数：");
	cin >> n;

	cout << endl;
	menu();
	cin >> a;

	while (a != 'B')
	{
		switch (a)
		{
			case 'I'://创建哈夫曼树
			{
				CreateTree(HT, n);//建立哈夫曼树函数
				printf("建立哈夫曼树成功，请选择下一步操作：\n\n");
				break;
			}
			case 'E'://编码
			{
				HuffmanCoding(HT, HC, n);//哈夫曼编码函数
				printf("编码成功，请选择下一步操作：\n\n");
				break;
			}
			case 'D'://译码
			{
				TranslateTree(HT, n);//哈夫曼译码函数
				printf("译码成功，请选择下一步操作：\n\n");
				break;
			}
			case 'P'://打印代码文件
			{
				Print();
				printf("打印代码过程完成!已将结果存入CodePrin.txt中.\n\n");
				break;
			}
			case 'T'://打印哈夫曼树
			{
				TreePrinting(HT, 2 * n - 1, 0);
				printf("\n打印哈夫曼树过程完成!同时已将结果存入TreePrint中.\n\n");
				break;
			}
			case 'B'://退出程序
			{
				return 0;
			}
			default:
			{
				printf("\n输入错误请重新输入：\n");
				break;
			}
		}
		cin >> a;
	}

	delete[]HT;
	return 0;
}