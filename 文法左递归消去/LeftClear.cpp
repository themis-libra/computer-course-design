#include<iostream>
#include<cmath>
#include<algorithm>
#include<map>
#include<vector>
#include<unordered_map> 
#include<set>
using namespace std;

struct node
{
	string left;
	set<string>right;
};
vector<node>v;
vector<string>notend;

bool allend(string str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			return false;
	}
	return true;
}

void getnotend()
{
	set<string>tempset;
	for (int i = 0; i < v.size(); i++)
	{
		tempset.insert(v[i].left);

	}

	set<string>::iterator it = tempset.begin();
	for (it; it != tempset.end(); it++)
	{
		notend.push_back(*it);
	}

}

vector<node> mysplit(string str)
{
	string vleft;
	set<string>vright;
	string temp = "";
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '>')continue;
		if (str[i] == '-')
		{
			vleft = temp;
			temp = "";
			continue;
		}
		if (str[i] == '|')
		{
			vright.insert(temp);
			temp = "";
			continue;
		}
		temp += str[i];
	}
	if (temp != "")vright.insert(temp);
	vector<node>ret;
	struct node N = { vleft,vright };
	ret.push_back(N);
	return ret;
}

void getproduction()
{
	printf("请输入产生式,以$为结束标志:\n");
	string str;
	while (cin >> str && str[0] != '$')
	{
		vector<node>ret = mysplit(str);
		v.push_back(ret[0]);
	}

}

string myreplace(string str, string s, string t)
{
	while (str.find(s) != -1)
	{
		int pos = str.find(s);
		str.replace(pos, s.size(), t);
	}
	return str;
}

void erasedirect(int posi)
{
	set<string>::iterator it = v[posi].right.begin();

	bool flag = true;
	for (it; it != v[posi].right.end(); it++)
	{
		string str = *it;
		if ((str.find(v[posi].left) == str.rfind(v[posi].left) && str.find(v[posi].left) == 0) || allend(str))
			flag = true;
		else
		{
			flag = false;
			break;
		}

	}
	if (!flag)return;

	vector<string>va, vb;
	for (it = v[posi].right.begin(); it != v[posi].right.end(); it++)
	{
		string str = *it;
		if (str.find(v[posi].left) == -1)
		{
			vb.push_back(str);
		}
		else
		{
			str.erase(0, v[posi].left.size());
			va.push_back(str);
		}
	}
	v[posi].right.clear();
	for (int i = 0; i < vb.size(); i++)
	{
		v[posi].right.insert(vb[i] + v[posi].left + "'");
	}

	set<string>ans;
	for (int i = 0; i < va.size(); i++)
	{
		ans.insert(va[i] + v[posi].left + "'");
	}
	ans.insert("Σ");
	struct node N = { v[posi].left + "'",ans };
	v.push_back(N);
}

void myoperate()
{
	for (int i = 0; i < notend.size(); i++)//FOR i:=1 TO N DO
	{
		for (int j = 0; j < i; j++)//FOR j:=1 TO i-1 DO
		{
			int posi = -1, posj = -1;
			for (int k = 0; k < v.size(); k++)
			{
				if (v[k].left == notend[i])/*遍历所有产生式，找到第i个非终结符在所有产生式中的位置*/
				{
					posi = k;
					break;
				}
			}
			for (int k = 0; k < v.size(); k++)/*遍历所有产生式，找到第i个非终结符在所有产生式中的位置*/
			{
				if (v[k].left == notend[j])
				{
					posj = k;
					break;
				}
			}
			if (posi == -1 || posj == -1)continue;/*如果找不到，就不执行下面的代码。例如：非终结符S在v中是第3个，非终结符R在v中是第1个，则posi=3,posj=1*/

			set<string>::iterator it = v[posi].right.begin();
			set<string>::iterator it2;
			set<string>tempset;
			for (it; it != v[posi].right.end(); it++)
			{
				string tempstr = *it;
				string ss;
				if (tempstr.find(notend[j]) != -1)
				{
					for (it2 = v[posj].right.begin(); it2 != v[posj].right.end(); it2++)
					{
						ss = tempstr;
						ss = myreplace(ss, notend[j], *it2);
						tempset.insert(ss);
					}

				}
			}
			vector<string>temv;
			for (it = v[posi].right.begin(); it != v[posi].right.end(); it++)
			{
				string str = *it;
				if (str.find(notend[j]) != -1)
					temv.push_back(str);
			}
			for (int x = 0; x < temv.size(); x++)
			{
				it = v[posi].right.find(temv[x]);
				if (it != v[posi].right.end())
					v[posi].right.erase(it);
			}
			for (it = tempset.begin(); it != tempset.end(); it++)
				v[posi].right.insert(*it);
			erasedirect(posi);

		}
	}
}

int main()
{
	getproduction();
	getnotend();
	myoperate();

	printf("消除左递归的结果为:\n");
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i].left << "->";
		set<string>::iterator it = v[i].right.begin();
		int cnt = 0;
		for (it; it != v[i].right.end(); it++)
		{
			cout << *it;
			cnt++;
			if (cnt != v[i].right.size())
				cout << '|';
		}
		cout << endl;
	}

	return 0;
}