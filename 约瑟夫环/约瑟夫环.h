#pragma once

#include<stdio.h>
#include<stdlib.h>
#define ELE int
typedef struct 	L	//链表
{
	ELE data;
	int seq;
	struct L* next;
}LNode, * LinkList;

//void menu()//菜单 
//{
//	printf("\n\n");
//	printf("\t\t\t\t********************************************\t\t\t\t\n");
//	printf("\t\t\t\t|        1:创建约瑟夫环-                     |\t\t\t\t\n");
//	printf("\t\t\t\t|        2:查看每个人的密码或最后出列人的密码|\t\t\t\t\n");
//	printf("\t\t\t\t|        3:输入一开始的报数m:                |\t\t\t\t\n");
//	printf("\t\t\t\t|        0:退出                              |\t\t\t\t\n");
//	printf("\t\t\t\t******************欢迎使用******************\t\t\t\t\n");
//
//}

//创建循环链表 
LinkList creat_list(LinkList head)
{
	int n, i, k;
	LinkList p = NULL;//进行定义指针时，没有申请空间，所以开始要赋予空值 
	free_list(head);//创建之前先销毁上一次的数据
	printf("请输入要创建的人数:");
	scanf("%d", &n);
	head = (LinkList)malloc(sizeof(LNode));//给首元结点 一个空间 
	p = head;//p指向第一个 把head的地址赋给p 
	for (i = 1; i < n; i++)
	{
		printf("输入第%d个人的密码:", i);
		scanf("%d", &p->data);//输入结点密码 
		p->seq = i;//结点序号 
		p->next = (LinkList)malloc(sizeof(LNode));
		p = p->next; //p往后走 
	}
	printf("输入第%d个人的密码:", i);
	scanf("%d", &p->data);
	p->seq = n;//最后一个结点序号为n 
	p->next = head;//首位相连 
	return head;
}

//销毁链表
void free_list(LinkList head)
{

	LinkList p = head, q = head;
	if (p == NULL)
		return;
	if (p->next == head)
	{
		free(p);
		head = NULL;
		return;
	}
	while (p->next != head)//最后的尾结点不指向头结点， 
	{
		q = p;//保存要删除结点的前一个地址 
		p = p->next;
		free(q);
	}
	free(p);
	free(head);
	head = NULL;

}

//输出链表元素
void show_list(LinkList head)
{
	LinkList p = head;//引用创建好的链表，head为头指针，头指针就可以找到创建的链表 
	if (p == NULL)
	{
		printf("无任何元素\n");
		return;
	}
	if (head->next == head)//如果它的下一个就是头，为一个结点 
	{
		printf("%d\n", head->data);
		return;
	}
	while (p->next != head)
	{
		printf("%d\t", p->data);//输出它的密码 
		p = p->next;
	}
	printf("%d\t", p->data);//输出最后一个 
	printf("\n");

}

//开始获取密码
LinkList kill_list(LinkList head)
{
	int n, i;
	LinkList p = head, q = head;//都为指针类型 

	if (head == NULL)
	{
		printf("无任何人\n");
		return NULL;
	}
	printf("输入一开始的报数m:");
	scanf("%d", &n);
	while (n <= 1)
	{
		printf("至少为2\n");
		scanf("%d", &n);
	}

	if (head->next == head)//如果只有一个元素
	{
		printf("%d", head->seq);
		return head;
	}
	printf("按照出列顺序打印各人的编号:\n\n");
	while (p->next != p)
	{
		for (i = 1; i < n; i++)//循环n次 
		{
			q = p; //q保存删除的前一个结点 
			p = p->next;//p移动到下一个位置，直到移动到位置为n,for循环结束时，p->next就为出列的结点 
		}
		q->next = p->next; //删除p的作用，此时的p->next为要删除结点的下一个地址 ， 

		printf("初始序号为%d的人出列，密码：%d\n", p->seq, p->data);//输出
		n = p->data;//新的密码 
		free(p);//释放该节点
		p = q->next;//更新p,q->next 为要删除结点的下一个地址 ，从此在该地址重新从1计数 

	}
	printf("最后一个人出列，");
	printf("密码：%d\t", p->seq);
	return p;
}



