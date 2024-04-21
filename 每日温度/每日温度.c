#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<malloc.h>

int main()
{
	int i=0, j=0;
	int n = 0;

	printf("ÇëÊäÈëÌìÊı£º");
	scanf("%d", &n);

    int* T = malloc(sizeof(int) * n);

	printf("temperatures=");
    for (i = 0; i < n; i++)
    {
        scanf("%d",&T[i]);
    }

    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (T[i] < T[j])
            {
                T[i] = j - i;
                break;
            }
            else if (j == n-1)
            {
                T[i] = 0;
            }
        }

    }
    T[n - 1] = 0;

    printf("[");
    for (i = 0; i < n-1; i++)
    {
        printf("%d ", T[i]);
    }
    printf("%d]",T[n-1]);

    return 0;
}