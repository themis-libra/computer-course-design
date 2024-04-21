#define _CRT_SECURE_NO_WARNINGS 1
//��ջģ��ͣ�������Զ���ģ�⳵����ı�������մ��ն˶���������������н���ģ�����
//ÿһ���������ݰ�������������:�������������ȥ����Ϣ���������պ����Լ��������ȥ��ʱ�̡�
//��ÿ���������ݽ��в�����������ϢΪ��
//���ǳ�����������������ͣ�����ڻ����ϵ�ͣ��λ�ã�
//���ǳ�����ȥ������������ڳ�����ͣ����ʱ���Ӧ���ɵķ���(�ڱ����ͣ����ʱ�䲻�շ�)

#include<iostream>
using namespace std;

#define m 50 //����ջ��Ԫ�ص�������
int Psite = 0;
int Qsite = 0;

/*����洢�ṹ*/
//˳��ջ 
typedef struct
{
    int carnum[m]; //���ƺ�
    int time[m];//������ʱ�� 
    int top;//ͣ��λ�ã�ջ��ָ�� 
}Sqstack;

//����
typedef struct Node
{
    int cn;//���ƺ��� 
    struct Node* next;//ָ���� 
}Qnode;//���ӽ��
typedef struct
{
    Qnode* head;//������͵Ķ�ͷָ�� 
    Qnode* tail;//������͵Ķ�βָ�� 
}LQueue; 

/*˳��ջ�Ĳ���*/
//��ʼ��
void InitStack(Sqstack* S) 
{
    S->top = -1;
}

//��ջ��
bool EmptyStack(Sqstack* S)
{
    if (S->top == -1)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

//��ջ�� 
bool FullStack(Sqstack* S,int n)
{
    if (S->top == n-1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//��ջ
void PushStack(Sqstack* S, int carnum, int time)
{
    if (S->top == m - 1)
    {
        cout << "ջ��" << endl;
        return;
    }
    S->top++;
    S->carnum[S->top] = carnum; //ָ���ȼ�һ������ջ
    S->time[S->top] = time;
    Psite++;
}

//��ջ
void PopStack(Sqstack* S, int &carnum, int& time)
{
    if (S->top == -1)
    {
        cout << "ջ��" << endl;
        return;
    }
    carnum = S->carnum[S->top]; //�ȳ�ջ��ָ���ټ�һ
    time = S->time[S->top];
    S->top--;
    Psite--;
}

//��ӡ����
void PrintStack(Sqstack* S) 
{
    for (int i = 0; i <= S->top; i++)//ѭ����� 
    {
        cout << S->carnum[i];
    }
    cout << endl;
}

/*���ӵĲ���*/
//��ʼ��
void InitQueue(LQueue* Q) 
{
    Q->head = Q->tail = NULL;
}

//���
void PushQueue(LQueue* Q, int x)
{
    Qnode* newnode = new Qnode;
    if (newnode == NULL)
    {
        cout << "������";
    }
    newnode->cn = x;
    newnode->next = NULL;

    //β��
    if (Q->head == NULL && Q->tail == NULL)
    {
        Q->head = Q->tail = newnode;
    }
    else
    {
        Q->tail->next = newnode;
        Q->tail = newnode;
    }
    Qsite++;
}

//����
void PopQueue(LQueue* Q)
{
    //ͷɾ
    if (Q->head->next == NULL)
    {
        delete Q->head;
        Q->head = Q->tail = NULL;
    }
    else
    {
        Qnode* next = Q->head->next;
        delete Q->head;
        Q->head = next;
    }
    Qsite--;
}

//�жӿ�
bool EmptyQueue(LQueue* Q)
{
    if (Q->head == NULL && Q->tail == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//���
void PrintQueue(LQueue* Q) 
{
    Qnode* temp;
    temp = Q->head->next;
    if (temp == NULL)
    {
        cout << "������";
    }
    else
    {
        while (temp != NULL)
        {
            cout << temp->cn;
            temp = temp->next;
        }
        cout << endl;
    }
}

int main()
{
    int e1=0,e2=0;
    int i, j, t;

    int n;//ͣ�������ͣ����
    char state;//����״̬-(A)���(D)��ȥ
    int carnum;//���ƺ�
    int time;//�����ȥ��ʱ��

    Sqstack P,P1;//P��ͣ������P1����ʱͣ�ų�
    LQueue Q;   //Qu�Ǻ򳵳�

    Q.head = new Qnode;//Ϊͷ���ָ���������ռ� 
    InitStack(&P);
    InitStack(&P1);
    InitQueue(&Q);

    cout << "������ͣ�������ͣ������";
    cin >> n;
    cout << endl;
    cout << "�����복��״̬�����ƺţ�ʱ�䣺";
    do
    {
        cin >> state;
        cin >> carnum >> time;

        /*��������*/
        if (state == 'A')
        {
            if (!FullStack(&P,n))//ͣ��������
            {
                PushStack(&P, carnum , time);//����ͣ����
                printf("ͣ������λ�ã�%d\n",Psite);
            }
            else//ͣ������
            {
                PushQueue(&Q, carnum);//������
                printf("���λ��:%d\n", Qsite);
            }
        }

        /*�����뿪*/
        if (state == 'D')
        {
            for (i = 0; i <= P.top && P.carnum[i] != carnum; i++);  //��ջ���� 
            if (i > P.top)//���ڱ����
            {
                printf("���ƺ�Ϊ%d������ͣ�ڱ���ϣ����շ�\n",carnum);
            }
            else//����ͣ������
            {
                t = P.top - i;  //��Ҫ��ջ�ĳ�����Ŀ
                for (j = 0; j < t; j++)  //��ͣ�Ĵ�ͣ������ջ���˳����������뵽��ʱջP1
                {
                    PopStack(&P, e1, e2);
                    PushStack(&P1, e1, e2);//e1��e2���Ǵ�Pop()�����еõ��� 
                }
                PopStack(&P, e1, e2);//�Ǹ���Ҫ��ȥ���������뿪 
                printf("���ƺ�Ϊ%d������ͣ��ʱ��Ϊ��%dСʱ��ͣ������Ϊ:%dԪ\n", carnum, time - e2,(time - e2) * 5);//ÿСʱ��Ԫ
                while (!EmptyStack(&P1))//����ʱջP1�еĳ��Ż�P��
                {
                    PopStack(&P1, e1, e2);
                    PushStack(&P, e1, e2);
                }
                if (!EmptyQueue(&Q))//�Ӳ���ʱ,����ͷ��ջP
                {
                    PushStack(&P, Q.head->cn, time);/*�Ե�ǰʱ�俪ʼ�Ʒ�*/
                    PopQueue(&Q);
                }
            }
        }
    } while (state != 'E');

    cout << "�������"<<endl;
    return 0;
}