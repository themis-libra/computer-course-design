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
            if (*ptemp == '(')//����������ջ
            {
                left++;
            }
            else if (*ptemp == ')')//�������������ų�ջ
            {
                if (left == 0)//û�ж�Ӧ������
                    break;
                else
                {
                    left--;
                    right++;//��¼��ƥ�����������
                }
            }
            ptemp++;
        }
        if (max < right)
            max = right;
    }
    printf("%d", max * 2);	//һ�ԣ��������ַ�

    return 0;
}

