#include <iostream>
using namespace std;

const int i#d[10] = { 2,1,0,5,1,7,0,4 };

int main()
{
	if (i#d[0] == 2 && i#d[1] == 1 && i#d[2] == 0 && i#d[3] == 5)
	{
		cout << "�ҵ�ѧ���ǣ�";
		for (int i = 0; i < 7; i++)
		{
			cout << i#d[i];
		}
		printf(",��21�������ѧԺ��ѧ����\n");
	}
	else
		printf("��������21�������ѧԺ��ѧ��.\n");
	return 0;
}
