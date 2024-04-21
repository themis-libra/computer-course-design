#include <stdio.h>
#include <string.h>

int firstUniqChar(char* s)
{
	int len = strlen(s);//计算字符串长度
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
	while ((ch = getchar()) != '\n')//一直接收缓冲区的字符。直至收到回车
	{
		s[i] = ch;
		i++;
	}
	s[i] = '\0';    //加上串尾符
	n=firstUniqChar(s);
	printf("%d\n", n);

	return 0;
}
