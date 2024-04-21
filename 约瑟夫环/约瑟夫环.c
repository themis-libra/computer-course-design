#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<malloc.h>
#include<assert.h>

typedef struct Node
{
    int id,password; //������
    struct Node* next; //ָ����
}Node, * PNode;

typedef struct List
{
    PNode first; //ָ��ͷ���
    PNode last; //ָ��β���
}List;

//��ʼ��ѭ��������
void InitSCList(List* list)
{
    list->first = list->last = NULL;
}

//��������
void pushlist(List* list, int id,int password)
{
    Node* p = list->first;
    Node* s = (Node*)malloc(sizeof(Node));
    if (id == 1) 
    {
        list->first = list->last = s;
        s->id = id;
        s->password = password;
    }
    else 
    {
        int j = 1;
        while (p->next != list->first && j < id - 1) //�ҵ�λ�ò���
        {
            p = p->next;
            j++;
        }
        if (j == id - 1) 
        {
            s->next = p->next;
            p->next = s;
            s->id = id;
            s->password = password;
            
            if (p == list->last)
                list->last = s;
        }
    }
    list->last->next = list->first;
}

//������ʾ
void show_list(List* list,int n)
{
    Node* p = list->first;//ָ����Ч��㲿��
    printf("��ǰѭ�������е�����Ԫ������Ϊ��\n\n");
    for (int i = 0; i < n; i++)
    {
        printf("λ��:%d ���룺%d\n", p->id, p->password);
        p = p->next;//����
    }
    printf("\n");
}

void Dequeue(List* list,int n,int m)
{
    Node* p = list->last;
    Node* pre;
    printf("����˳��Ϊ��\n");
    int i = 0;
    for (i = n; i > 0; i--)
    {
        while (m - 1)
        {
            p = p->next;
            m--;
        }
        for (int j = 1; j < m-1; j++)
            p = p->next;
        pre = p->next;
        p->next = pre->next;
        printf("%d ", pre->id);
        m = pre->password;
        free(pre);
    }
    list->first = list->last = NULL;
}

int main()
{
    List mylist;
    InitSCList(&mylist);

    int n, m;
    printf("��Լɪ�򻷡�\n\n");
    printf("\n����������:");
    scanf("%d", &n);
    printf("\n");

    for (int i = 1, k = 0; i <= n; i++)//��������
    {
        printf("\n�������%d���˵�����:", i);
        scanf("%d", &k);
        pushlist(&mylist, i, k);
    }

    printf("\n\n");
    show_list(&mylist,n);

    printf("\n�������ʼ���룺");//��ʼʱҪ������
    scanf("%d", &m);
    printf("\n\n");

    Dequeue(&mylist, n, m);

    printf("\n\n");

    return 0;
}