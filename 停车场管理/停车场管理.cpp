#define _CRT_SECURE_NO_WARNINGS 1
//以栈模拟停车场，以队列模拟车场外的便道，按照从终端读入的输入数据序列进行模拟管理。
//每一组输入数据包括三个数据项:汽车“到达”或“离去”信息、汽车牌照号码以及到达或离去的时刻。
//对每组输入数据进行操作后的输出信息为：
//若是车辆到达，则输出汽车在停车场内或便道上的停车位置；
//若是车辆离去，则输出汽车在车场内停留的时间和应交纳的费用(在便道上停留的时间不收费)

#include<iostream>
using namespace std;

#define m 50 //定义栈中元素的最大个数
int Psite = 0;
int Qsite = 0;

/*定义存储结构*/
//顺序栈 
typedef struct
{
    int carnum[m]; //车牌号
    int time[m];//进出入时间 
    int top;//停车位置，栈顶指针 
}Sqstack;

//链队
typedef struct Node
{
    int cn;//车牌号码 
    struct Node* next;//指针域 
}Qnode;//链队结点
typedef struct
{
    Qnode* head;//结点类型的队头指针 
    Qnode* tail;//结点类型的队尾指针 
}LQueue; 

/*顺序栈的操作*/
//初始化
void InitStack(Sqstack* S) 
{
    S->top = -1;
}

//判栈空
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

//判栈满 
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

//入栈
void PushStack(Sqstack* S, int carnum, int time)
{
    if (S->top == m - 1)
    {
        cout << "栈满" << endl;
        return;
    }
    S->top++;
    S->carnum[S->top] = carnum; //指针先加一，再入栈
    S->time[S->top] = time;
    Psite++;
}

//出栈
void PopStack(Sqstack* S, int &carnum, int& time)
{
    if (S->top == -1)
    {
        cout << "栈空" << endl;
        return;
    }
    carnum = S->carnum[S->top]; //先出栈，指针再减一
    time = S->time[S->top];
    S->top--;
    Psite--;
}

//打印数据
void PrintStack(Sqstack* S) 
{
    for (int i = 0; i <= S->top; i++)//循环输出 
    {
        cout << S->carnum[i];
    }
    cout << endl;
}

/*链队的操作*/
//初始化
void InitQueue(LQueue* Q) 
{
    Q->head = Q->tail = NULL;
}

//入队
void PushQueue(LQueue* Q, int x)
{
    Qnode* newnode = new Qnode;
    if (newnode == NULL)
    {
        cout << "出错了";
    }
    newnode->cn = x;
    newnode->next = NULL;

    //尾插
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

//出队
void PopQueue(LQueue* Q)
{
    //头删
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

//判队空
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

//输出
void PrintQueue(LQueue* Q) 
{
    Qnode* temp;
    temp = Q->head->next;
    if (temp == NULL)
    {
        cout << "出错了";
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

    int n;//停车场最大停车数
    char state;//车辆状态-(A)到达、(D)离去
    int carnum;//车牌号
    int time;//到达、离去的时间

    Sqstack P,P1;//P是停车场，P1是临时停放场
    LQueue Q;   //Qu是候车场

    Q.head = new Qnode;//为头结点指针申请分配空间 
    InitStack(&P);
    InitStack(&P1);
    InitQueue(&Q);

    cout << "请输入停车场最大停车数：";
    cin >> n;
    cout << endl;
    cout << "请输入车辆状态，车牌号，时间：";
    do
    {
        cin >> state;
        cin >> carnum >> time;

        /*汽车到达*/
        if (state == 'A')
        {
            if (!FullStack(&P,n))//停车场不满
            {
                PushStack(&P, carnum , time);//进入停车场
                printf("停车场内位置：%d\n",Psite);
            }
            else//停车场满
            {
                PushQueue(&Q, carnum);//进入便道
                printf("便道位置:%d\n", Qsite);
            }
        }

        /*汽车离开*/
        if (state == 'D')
        {
            for (i = 0; i <= P.top && P.carnum[i] != carnum; i++);  //在栈中找 
            if (i > P.top)//车在便道内
            {
                printf("车牌号为%d的汽车停在便道上，不收费\n",carnum);
            }
            else//车在停车场内
            {
                t = P.top - i;  //需要出栈的车辆数目
                for (j = 0; j < t; j++)  //不停的从停车场的栈中退出车辆，进入到临时栈P1
                {
                    PopStack(&P, e1, e2);
                    PushStack(&P1, e1, e2);//e1和e2都是从Pop()方法中得到的 
                }
                PopStack(&P, e1, e2);//那个想要出去的汽车，离开 
                printf("车牌号为%d的汽车停留时间为：%d小时，停车费用为:%d元\n", carnum, time - e2,(time - e2) * 5);//每小时五元
                while (!EmptyStack(&P1))//将临时栈P1中的车放回P中
                {
                    PopStack(&P1, e1, e2);
                    PushStack(&P, e1, e2);
                }
                if (!EmptyQueue(&Q))//队不空时,将队头进栈P
                {
                    PushStack(&P, Q.head->cn, time);/*以当前时间开始计费*/
                    PopQueue(&Q);
                }
            }
        }
    } while (state != 'E');

    cout << "输入结束"<<endl;
    return 0;
}