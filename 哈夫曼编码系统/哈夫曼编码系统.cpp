#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string>
#include <fstream>
#include <iostream>
using namespace std;

#define number 6   //�ַ�����

/*���������洢�ṹ����*/
typedef struct
{
	int weight;					//Ȩֵ
	char ch;					//����Ӧ�ַ�
	int parent, lchild, rchild;	//����˫�׺����Һ���
}HTNode, * HuffmanTree;			//��̬��������洢��������

typedef char** Huffmancode;//��̬��������洢����������

/*ѡ����˫����Ȩֵ��С���������*/
void Select(HuffmanTree HT, int n, int& s1, int& s2)
{
	int i = 1, min = 0;

	//���ҵ���һ��˫��Ϊ-1�Ľ���Ȩֵ��ʱ��Ϊ��Сֵ�����������бȽ�
	for (i = 1; i <= n; i++)
	{
		if (HT[i].parent == -1)
		{
			min = i;
			break;
		}
	}
	//���ϸ�ѭ���ҵ�����ʱ����Сֵ��ÿ������Ȩֵ���бȽϴӶ��ҳ�Ȩֵ��С�Ľ��
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
	s1 = min;//��s1ΪȨֵ��С�Ľ��

	//Ѱ��Ȩֵ��С�Ľ��
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
	s2 = min;//��s2Ϊ�ڶ���Ȩֵ��С�Ľ��
}

/*��ʼ����������������*/
void CreateTree(HTNode* HT, int n)
{
	int m, i, s1, s2;
	
	//�ַ���ΪҶ�ӽ��,�ɶ����������ʣ���Ϊ2�Ľ����+1=Ҷ�ӽڵ����ɵ��ܽ�����
	m = 2 * n - 1;

	//����forѭ�����������������н���Ȩֵ��˫�׺����Һ��Ӹ�ֵΪ0
	for (i = 1; i < m+1; i++)
	{
		HT[i].ch = '@';
		HT[i].weight = 0;
		HT[i].parent = -1;
		HT[i].lchild = -1;
		HT[i].rchild = -1;
	}

	printf("�������ַ�����Ȩֵ��\n");

	//����ѭ��ΪҶ�ӽ�㸳Ȩֵ���ַ�
	i = 1;
	while (i <= n)
	{
		getchar();

		HT[i].ch = getchar();
		cin >> HT[i].weight;
		i++;
	}

	//������������
	for (i = n + 1; i < m+1; i++)
	{
		Select(HT, i - 1, s1, s2);//��HT[1...i-1]��ѡ����˫����Ȩֵ��С���������
		HT[s1].parent = i;//s1��s2��ϣ���i�������ǵ�˫����
		HT[s2].parent = i;
		HT[i].lchild = s1;//��s1��s2����i����������Һ�����
		HT[i].rchild = s2;
		//i��Ȩֵ���������������ӵ�Ȩֵ���
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}

	FILE* fp1;
	fp1 = fopen("hfmTree.txt", "w");
	if (fp1 == NULL)
	{
		printf("�ļ���ʧ��\n");
		exit(0);
	}
	//�������õĹ������������ļ���
	for (i = 1; i <= m; i++)
	{
		fprintf(fp1, "%c %d %d %d %d\n", HT[i].ch, HT[i].weight,HT[i].parent, HT[i].lchild, HT[i].rchild );
	}
	fclose(fp1);
}

/*Ҫ������ļ�*/
void ToBeTran()
{
	char s[50];//��ű���
	printf("������Ҫ���б���ı���:\n");
	getchar();
	cin.getline(s, 50);

	FILE* fp;
	fp = fopen("ToBeTran.txt", "w");
	if ( fp == NULL)
	{
		printf("�ļ���ʧ��\n");
		exit(0);
	}

	for (int i = 0; s[i] != '\0'; i++)
	{
		fprintf(fp, "%c", s[i]/*Data[i].c*/);
	}
	fprintf(fp, "#");
	fclose(fp);
}

/*���뺯��*/
void HuffmanCoding(HTNode* HT, Huffmancode& HC, int n)
{   //HCʵ��Ϊ�洢ÿ�������ͷָ������
	//����n���ַ������ͷָ��洢����ռ䣬0�ŵ�Ԫ����
	HC = (Huffmancode)malloc((number + 1) * sizeof(char*));
	int i, start, c, f;
	char* cd;//������ʱ�洢����Ĺ����ռ�
	cd = (char*)malloc(n * sizeof(char));
	cd[n - 1] = '\0';//���������
	printf("ÿ���ַ������Ӧ�������£�\n");
	for (i = 1; i <= n; i++)
	{
		//��Ϊ�Ǵ�Ҷ�ӽ��������б��룬����startָ��洢����Ľ�����λ��
		start = n - 1;
		//��Ҷ�ӽ�㵽�����������
		f = HT[i].parent;
		c = i;
		while (f != -1)
		{
			--start;
			if (HT[f].lchild == c)	//���Ŀǰ�����f�����������Ϊ0
				cd[start] = '0';
			else					//���Ŀǰ�����f���Һ��������Ϊ1
				cd[start] = '1';

			//������cָ��Ŀǰ����˫�ף�fָ��Ŀǰ����˫�׵�˫�ײ��Ͻ��б���
			//ֱ��fָ������
			c = f;
			f = HT[f].parent;
		}
		//��ÿ��Ҷ�ӽ����б������֮��㽫��Ӧ�ı���洢��HC��
		//Ϊ��i���ַ��ı������ռ�
		HC[i] = (char*)malloc((number - start) * sizeof(char));
		strcpy(HC[i], &cd[start]);//��cd���Ƶ�HC
		printf("%c--%s\t", HT[i].ch, HC[i]);//����ַ������Ӧ����
	}
	free(cd);//�ͷ���ʱ�洢����Ĺ����ռ�

	cout << endl;
	ToBeTran();//���뱨��

	FILE* fp, * fp1;
	fp = fopen("ToBeTran.txt", "r");
	fp1 = fopen("CodeFile.txt", "w");

	//����CodeFile.txt�ļ����ڴ�ű���
	if (fp1 == NULL)
	{
		printf("�ļ���ʧ��\n");
		exit(0);
	}

	//�ж�ToBeTran.txt�ļ��Ƿ����
	if (fp == NULL)
	{
		printf("�ļ���ȡʧ��\n");
		exit(0);
	}
	c = fgetc(fp);
	while (c != '#')
	{
		for (i = 1; i <= n; i++)
		{
			if (c == HT[i].ch) //�ҵ��ַ����,����Ӧ�Ĺ���������д���ļ�CodeFile.txt��
				fprintf(fp1, "%s", HC[i]);
		}
		c = fgetc(fp);
	}
	fprintf(fp1, "#");
	fclose(fp);
	fclose(fp1);
}

/*���뺯��*/
void TranslateTree(HTNode* HT, int n)//�Ӹ����������ַ�'0'/'1'��Ѱ�����ӻ����Һ���ֱ��Ҷ�ӽ��
{
	int m;
	char b;

	FILE* fp, * fp1, * fp2;
	fp = fopen("CodeFile.txt", "r");
	fp1 = fopen("TextFile.txt", "w");
	fp2 = fopen("TextFile.txt", "r");

	if (fp == NULL)
	{
		printf("�ļ���ʧ��\n");
		exit(0);
	}

	//����������������TextFile
	if (fp1 == NULL)
	{
		printf("�ļ���ʧ��\n");
		exit(0);
	}

	printf("�ַ�����������Ľ��Ϊ��\n");
	b = fgetc(fp); //��CodeFile�ж�ȡ�ַ�
	m = 2 * n - 1;//��ʼ����mָ�����㣬�Ӹ�������
	while (b != '#')
	{
		if (b == '0')		//���Ϊ'0'����ָ��������
		{
			m = HT[m].lchild;
		}
		else if (b == '1')	//���Ϊ'1'����ָ�����Һ���
		{
			m = HT[m].rchild;
		}
		//һֱ��������򽫸ý���ʾ���ַ������ļ��в���m����ָ��������в���
		if (HT[m].lchild == -1)
		{
			fprintf(fp1, "%c", HT[m].ch);
			m = 2 * n - 1;
		}
		printf("%c", b);
		b = fgetc(fp);//�ٶ���һ���ַ�
	}
	printf("����");
	fprintf(fp1, "#"); //����#��д���ļ�
	fclose(fp);
	fclose(fp1);

	if (fp2 == NULL)
	{
		printf("�ļ���ʧ��\n");
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

/*��ӡ�����ļ�*/
void Print() 
{
	FILE* fp1, * fp2;
	fp1 = fopen("CodeFile.txt", "r");
	fp2 = fopen("CodePrin.txt", "w+");

	int count = 0;
	char ch;
	ch = fgetc(fp1);
	while (ch != '#') //�ļ�δ��ĩβ����ѭ��
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

/*��ӡ��������*/
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
	printf("--------���������������ϵͳ------\n");
	printf("I.������������\n");	
	printf("E.����\n");			
	printf("D.����\n");			
	printf("P.��ӡ�����ļ�\n");	
	printf("T.��ӡ��������\n");	
	printf("B.�˳�����\n");		
	printf("----------------------------------\n");
	printf("�������ѡ��");
}

int main()
{
	int n;
	char a;

	//����m+1�����Ĵ洢�ռ䣬����0�ŵ�λ����
	HTNode* HT = new HTNode[2 * number];
	Huffmancode HC = NULL;

	printf("�������ַ�������");
	cin >> n;

	cout << endl;
	menu();
	cin >> a;

	while (a != 'B')
	{
		switch (a)
		{
			case 'I'://������������
			{
				CreateTree(HT, n);//����������������
				printf("�������������ɹ�����ѡ����һ��������\n\n");
				break;
			}
			case 'E'://����
			{
				HuffmanCoding(HT, HC, n);//���������뺯��
				printf("����ɹ�����ѡ����һ��������\n\n");
				break;
			}
			case 'D'://����
			{
				TranslateTree(HT, n);//���������뺯��
				printf("����ɹ�����ѡ����һ��������\n\n");
				break;
			}
			case 'P'://��ӡ�����ļ�
			{
				Print();
				printf("��ӡ����������!�ѽ��������CodePrin.txt��.\n\n");
				break;
			}
			case 'T'://��ӡ��������
			{
				TreePrinting(HT, 2 * n - 1, 0);
				printf("\n��ӡ���������������!ͬʱ�ѽ��������TreePrint��.\n\n");
				break;
			}
			case 'B'://�˳�����
			{
				return 0;
			}
			default:
			{
				printf("\n����������������룺\n");
				break;
			}
		}
		cin >> a;
	}

	delete[]HT;
	return 0;
}