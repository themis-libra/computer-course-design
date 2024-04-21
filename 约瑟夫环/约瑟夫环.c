#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<malloc.h>
#include<assert.h>

typedef struct Node
{
    int id,password; //数据域
    struct Node* next; //指针域
}Node, * PNode;

typedef struct List
{
    PNode first; //指向头结点
    PNode last; //指向尾结点
}List;

//初始化循环单链表
void InitSCList(List* list)
{
    list->first = list->last = NULL;
}

//插入数据
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
        while (p->next != list->first && j < id - 1) //找到位置插入
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

//数据显示
void show_list(List* list,int n)
{
    Node* p = list->first;//指向有效结点部分
    printf("当前循环链表中的数据元素依序为：\n\n");
    for (int i = 0; i < n; i++)
    {
        printf("位置:%d 密码：%d\n", p->id, p->password);
        p = p->next;//下移
    }
    printf("\n");
}

void Dequeue(List* list,int n,int m)
{
    Node* p = list->last;
    Node* pre;
    printf("出列顺序为：\n");
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
    printf("《约瑟夫环》\n\n");
    printf("\n请输入人数:");
    scanf("%d", &n);
    printf("\n");

    for (int i = 1, k = 0; i <= n; i++)//输入数据
    {
        printf("\n请输入第%d个人的密码:", i);
        scanf("%d", &k);
        pushlist(&mylist, i, k);
    }

    printf("\n\n");
    show_list(&mylist,n);

    printf("\n请输入初始密码：");//开始时要数的数
    scanf("%d", &m);
    printf("\n\n");

    Dequeue(&mylist, n, m);

    printf("\n\n");

    return 0;
}