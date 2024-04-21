#pragma once

#include<stdio.h>
#include<stdlib.h>
#define ELE int
typedef struct 	L	//����
{
	ELE data;
	int seq;
	struct L* next;
}LNode, * LinkList;

//void menu()//�˵� 
//{
//	printf("\n\n");
//	printf("\t\t\t\t********************************************\t\t\t\t\n");
//	printf("\t\t\t\t|        1:����Լɪ��-                     |\t\t\t\t\n");
//	printf("\t\t\t\t|        2:�鿴ÿ���˵�������������˵�����|\t\t\t\t\n");
//	printf("\t\t\t\t|        3:����һ��ʼ�ı���m:                |\t\t\t\t\n");
//	printf("\t\t\t\t|        0:�˳�                              |\t\t\t\t\n");
//	printf("\t\t\t\t******************��ӭʹ��******************\t\t\t\t\n");
//
//}

//����ѭ������ 
LinkList creat_list(LinkList head)
{
	int n, i, k;
	LinkList p = NULL;//���ж���ָ��ʱ��û������ռ䣬���Կ�ʼҪ�����ֵ 
	free_list(head);//����֮ǰ��������һ�ε�����
	printf("������Ҫ����������:");
	scanf("%d", &n);
	head = (LinkList)malloc(sizeof(LNode));//����Ԫ��� һ���ռ� 
	p = head;//pָ���һ�� ��head�ĵ�ַ����p 
	for (i = 1; i < n; i++)
	{
		printf("�����%d���˵�����:", i);
		scanf("%d", &p->data);//���������� 
		p->seq = i;//������ 
		p->next = (LinkList)malloc(sizeof(LNode));
		p = p->next; //p������ 
	}
	printf("�����%d���˵�����:", i);
	scanf("%d", &p->data);
	p->seq = n;//���һ��������Ϊn 
	p->next = head;//��λ���� 
	return head;
}

//��������
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
	while (p->next != head)//����β��㲻ָ��ͷ��㣬 
	{
		q = p;//����Ҫɾ������ǰһ����ַ 
		p = p->next;
		free(q);
	}
	free(p);
	free(head);
	head = NULL;

}

//�������Ԫ��
void show_list(LinkList head)
{
	LinkList p = head;//���ô����õ�����headΪͷָ�룬ͷָ��Ϳ����ҵ����������� 
	if (p == NULL)
	{
		printf("���κ�Ԫ��\n");
		return;
	}
	if (head->next == head)//���������һ������ͷ��Ϊһ����� 
	{
		printf("%d\n", head->data);
		return;
	}
	while (p->next != head)
	{
		printf("%d\t", p->data);//����������� 
		p = p->next;
	}
	printf("%d\t", p->data);//������һ�� 
	printf("\n");

}

//��ʼ��ȡ����
LinkList kill_list(LinkList head)
{
	int n, i;
	LinkList p = head, q = head;//��Ϊָ������ 

	if (head == NULL)
	{
		printf("���κ���\n");
		return NULL;
	}
	printf("����һ��ʼ�ı���m:");
	scanf("%d", &n);
	while (n <= 1)
	{
		printf("����Ϊ2\n");
		scanf("%d", &n);
	}

	if (head->next == head)//���ֻ��һ��Ԫ��
	{
		printf("%d", head->seq);
		return head;
	}
	printf("���ճ���˳���ӡ���˵ı��:\n\n");
	while (p->next != p)
	{
		for (i = 1; i < n; i++)//ѭ��n�� 
		{
			q = p; //q����ɾ����ǰһ����� 
			p = p->next;//p�ƶ�����һ��λ�ã�ֱ���ƶ���λ��Ϊn,forѭ������ʱ��p->next��Ϊ���еĽ�� 
		}
		q->next = p->next; //ɾ��p�����ã���ʱ��p->nextΪҪɾ��������һ����ַ �� 

		printf("��ʼ���Ϊ%d���˳��У����룺%d\n", p->seq, p->data);//���
		n = p->data;//�µ����� 
		free(p);//�ͷŸýڵ�
		p = q->next;//����p,q->next ΪҪɾ��������һ����ַ ���Ӵ��ڸõ�ַ���´�1���� 

	}
	printf("���һ���˳��У�");
	printf("���룺%d\t", p->seq);
	return p;
}



