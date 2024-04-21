#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<string>
#include<malloc.h>

char pairs(char a) 
{
    if (a == '}') return '{';
    if (a == ']') return '[';
    if (a == ')') return '(';
    return 0;
}

int isValid(char* s,int n) 
{
    if (n % 2 == 1) 
    {
        return 0;
    }

    int* stk = (int*)malloc((n+1)*sizeof(int));
    int top = 0;
    for (int i = 0; i < n; i++) 
    {
        char ch = pairs(s[i]);
        if (ch) 
        {
            if (top == 0 || stk[top - 1] != ch) 
            {
                return 0;
            }
            top--;
        }
        else 
        {
            stk[top++] = s[i];
        }
    }
    free(stk);
    return 1;
}

int main()
{
    int n,i,k=0;
    printf("ÇëÊäÈë×Ö·û´®³¤¶È£º");
    scanf("%d",&n);
    printf("\n");

    char* s = (char*)malloc(n  * sizeof(char));
    printf("ÇëÊäÈë×Ö·û´®£º");
    getchar();
    for (i = 0; i < n; i++)
    {
        scanf("%c", &s[i]);
    }

    k = isValid(s,n);
    if (k == 1)
        printf("false");
    else
        printf("true");

    return 0;
}

