#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    char S[50] = { 0 };
    gets(S);

    char* plocal = S;
    char* ptemp;

    int max = 0;
    int right;
    int left = 0;

    while (*plocal != '\0')
    {
        right = 0;
        ptemp = plocal;
        plocal++;
        while (*ptemp != '\0')
        {
            if (*ptemp == '(')//左括号则入栈
            {
                left++;
            }
            else if (*ptemp == ')')//右括号则将左括号出栈
            {
                if (left == 0)//没有对应左括号
                    break;
                else
                {
                    left--;
                    right++;//记录已匹配的右括号数
                }
            }
            ptemp++;
        }
        if (max < right)
            max = right;
    }
    printf("%d", max * 2);	//一对（）两个字符

    return 0;
}

