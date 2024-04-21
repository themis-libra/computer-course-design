#include <iostream>
using namespace std;

const int id[10] = { 2,1,0,5,1,7,0,4 };

int main()
{
	if (id[0] == 2 && id[1] == 1 && id[2] == 0 && id[3] == 5)
	{
		cout << "我的学号是：";
		for (int i = 0; i < 7; i++)
		{
			cout << id[i];
		}
		printf(",是21级计算机学院的学生。\n");
	}
	else
		printf("该生不是21级计算机学院的学生.\n");
	return 0;
}
