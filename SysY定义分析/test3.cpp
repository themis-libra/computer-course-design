#include <iostream>
using namespace std;

int main()
{
	const int a = 10;
	int b = a-10;
	int i;
	for (i = 0; i < 10; )
	{
		b = b + 2;
		i = i + 2;
	}
	if (a != b)
	{
		cout << "a!=b";
	}
	else cout << "a=b";
	return 0;
}
