#include <stdio.h>
#include <string.h>

int firstUniqChar(char* s)
{
	int len = strlen(s);//�����ַ�������
	int flag = 0;
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			if (s[i] == s[j] && i != j)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 0)
			return i;
		else
			flag = 0;
	}
	return -1;
}

int main()
{
	int i = 0;
	int n = 0;
	char ch;
	char s[50]={0};
	printf("s=");
	while ((ch = getchar()) != '\n')//һֱ���ջ��������ַ���ֱ���յ��س�
	{
		s[i] = ch;
		i++;
	}
	s[i] = '\0';    //���ϴ�β��
	n=firstUniqChar(s);
	printf("%d\n", n);

	return 0;
}
