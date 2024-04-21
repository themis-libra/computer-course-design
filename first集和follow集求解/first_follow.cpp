#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
using namespace std;

// �ؼ��ֱ��ó�ʼֵ
string Cppkeyword[100] = { "#", "��ʶ��(������)", "����", "ʵ��", "�ַ�����", "+", "-", "*", "/", "<",
                          "<=", "==", "!=", ">", ">=", "&", "&&", "||", "=", "(",
                          ")", "[", "]", "{", "}", ":", ";", ",", "void", "int",
                          "float", "char", "if", "else", "while", "do", "for", "include", "iostream", "using",
                          "namespace", "std", "main", "return", "null" };

map<string, bool> nullable;      //���ս������ս���Ƿ�ɿ�
vector<string> grammar;          //�洢�ķ�
set<string> first[50];           //�洢��Vn��Vt��first����û��VtҲ����first��������������
set<string> follow[50];          //�洢��Vn��Vt��follow����Vt��follow���ǿգ�266~288��ȡ��ע�Ϳɲ鿴�ս����first��follow��
string VN[50], VT[50];           //���ս�����ս��
map<string, int> VN2int, VT2int; //VN��VTӳ��Ϊ�±�����
int symbolNum = 0;
int PreAnalyseTable[50][50]; //Ԥ�������

void initGrammar()
{ //�ķ�����������ɾ������Ҫ��"A->a|b"��ʽ�ģ�����а����ǲ������:"A->a"��"A->b";�ռ�����null����ʾ
    grammar.push_back("E->TE'");
    grammar.push_back("E'->+TE'");
    grammar.push_back("E'->null");
    grammar.push_back("T->FT'");
    grammar.push_back("T'->*FT'");
    grammar.push_back("T'->null");
    grammar.push_back("F->(E)");
    grammar.push_back("F->i");
}

string getVn(string grammar)
{ //��ȡ�ķ��еķ��ս��
    if (grammar[1] == '\'')
    { //��'�ķ��ս��,�� E',T'
        return grammar.substr(0, 2);
    }
    else
    { //����'���������ս������E��T
        return grammar.substr(0, 1);
    }
}

string getVt(string grammar)
{ //��ȡ�ķ��е��ս��
    //Cppkeyword[1] = "i";
    for (int k = 1; k <= 44; k++)
    {
        string Vt = grammar.substr(0, Cppkeyword[k].length());
        if (Vt == Cppkeyword[k])
        {
            return Vt;
        }
    }
    //������е����˵������ս�����ǹؼ��ʱ���ģ���ΪСд��ĸҲ�����ս��
    if (grammar[0] >= 'a' && grammar[0] <= 'z')
    {
        return grammar.substr(0, 1);
    }
}

void readVnAndVt()
{
    //ɨ��һ������ʽ��ʶ�����еķ��ս�����ս��
    for (int i = 0; i < grammar.size(); i++)
    {
        for (int j = 0; j < grammar[i].length(); j++)
        {
            if (grammar[i][j] >= 'A' && grammar[i][j] <= 'Z')
            { //���ս��һ���д
                string Vn = getVn(grammar[i].substr(j, 2));
                if (VN2int[Vn] == 0)
                {
                    VN2int[Vn] = ++symbolNum;
                    VN[symbolNum] = Vn; //��ͨ�������±������Ӧ���ս������
                }
                j = j + Vn.length() - 1;
            }
            else if (grammar[i].substr(j, 2) == "->")
            {
                j = j + 2 - 1;
            }
            else
            { //ɨ�����ʽ�Ҳ��Ŀ��ܵ��ս��(�ؼ��ʱ�)
                string Vt = getVt(grammar[i].substr(j, grammar[i].length() - j));

                if (VT2int[Vt] == 0)
                { //���ս����һ�γ���,�����ս��ӳ��Ϊ�±�����
                    VT2int[Vt] = ++symbolNum;
                    VT[symbolNum] = Vt; //��ͨ�������±������Ӧ�ս������
                }
                j = j + Vt.length() - 1;
            }
        }
    }
}

bool allNullable(vector<string> Y, int left, int right)
{ //�ж� Y[left]...Y[right]�Ƿ�ȫ�ɿ�
    if (left >= Y.size() || left > right || right < 0)
        return true;
    for (int i = left; i <= right; i++)
    {
        if (nullable[Y[i]] == false)
            return false;
    }
    return true;
}

vector<string> splitGrammarIntoYi(string rightGrama)
{ //������ʽ���Ҳ�(��->�Ҳ����)��X->Y1Y2...Yk
    vector<string> Y;
    for (int j = 0; j < rightGrama.length(); j++)
    {
        if (rightGrama[j] >= 'A' && rightGrama[j] <= 'Z')
        { //���ս��
            string Vn = getVn(rightGrama.substr(j, 2));
            Y.push_back(Vn);
            j = j + Vn.length() - 1;
        }
        else
        { //�ս��
            string Vt = getVt(rightGrama.substr(j, rightGrama.length() - j));
            Y.push_back(Vt);
            j = j + Vt.length() - 1;
        }
    }
    return Y;
}

void getFirstFollowSet()
{
    /*����FIRST��FOLLOW��nullable���㷨*/
    for (auto it = VT2int.begin(); it != VT2int.end(); it++)
    { //��ÿһ���ս��Z��first[Z]={Z}
        string Vt = it->first;
        int Vt_index = it->second;
        first[Vt_index].insert(Vt);
    }
    for (int grammarIndex = 0; grammarIndex < grammar.size(); grammarIndex++)
    {
        //����ÿ������ʽ��X->Y1Y2...Yk
        string X;
        vector<string> Y;
        int delimiterIndex = grammar[grammarIndex].find("->");
        X = grammar[grammarIndex].substr(0, delimiterIndex);                                                                       //��"->"Ϊ�磬�ָ�����ʽ
        string rightGrama = grammar[grammarIndex].substr(delimiterIndex + 2, grammar[grammarIndex].length() - delimiterIndex - 2); //��ȡ�󲿲���ʽ
        Y = splitGrammarIntoYi(rightGrama);

        int k = Y.size();
        nullable["null"] = true;
        //�������Yi���ǿɿյģ���nullable[X]=true
        if (allNullable(Y, 0, k - 1))
        {
            nullable[X] = true;
        }

        for (int i = 0; i < k; i++)
        {
            //���Y0...Y(i-1)���ǿɿյ�(����֮��Yi����),��first[X] = first[X]��first[Yi] (1)
            if (nullable[Y[i]] == false && allNullable(Y, 0, i - 1))
            {
                if (i <= k - 1)
                {
                    set<string> setX = first[VN2int[X]];
                    //�ж�Yi���ս�����Ƿ��ս��
                    set<string> setY = VT2int.count(Y[i]) != 0 ? first[VT2int[Y[i]]] : first[VN2int[Y[i]]];
                    set_union(setX.begin(), setX.end(), setY.begin(), setY.end(), inserter(setX, setX.begin())); //(1)
                    first[VN2int[X]] = setX;
                }
            }
            //���Y(i+1)...Yk���ǿɿյ�(����֮��Y0..Y(i-1)������)����follow[Yi] = follow[Yi]��follow[X] (2)
            if (allNullable(Y, i + 1, k - 1))
            {
                set<string> setX = follow[VN2int[X]];
                //�ж�Yi���ս�����Ƿ��ս��
                set<string> setY = VT2int.count(Y[i]) ? follow[VT2int[Y[i]]] : follow[VN2int[Y[i]]];
                set_union(setX.begin(), setX.end(), setY.begin(), setY.end(), inserter(setY, setY.begin()));
                VT2int.count(Y[i]) ? follow[VT2int[Y[i]]] : follow[VN2int[Y[i]]] = setY;
            }

            for (int j = i + 1; j < k; j++)
            {
                //���Y(i+1)...Y(j-1)���ǿɿյ�(����֮��Yj����),��follow[Yi] = follow[Yi]��first[Yj] (3)
                if (nullable[Y[j]] == false && allNullable(Y, i + 1, j - 1))
                {
                    if (j <= k - 1)
                    {
                        set<string> setYi = VT2int.count(Y[i]) ? follow[VT2int[Y[i]]] : follow[VN2int[Y[i]]];
                        set<string> setYj = VT2int.count(Y[j]) ? first[VT2int[Y[j]]] : first[VN2int[Y[j]]];
                        set_union(setYi.begin(), setYi.end(), setYj.begin(), setYj.end(), inserter(setYi, setYi.begin()));
                        VT2int.count(Y[i]) ? follow[VT2int[Y[i]]] : follow[VN2int[Y[i]]] = setYi;
                    }
                }
            }
        }
    }
}

void converge()
{
    set<string> oldFirst[50];
    set<string> oldFollow[50];
    int isConverge = 1;
    follow[VN2int["E"]].insert("#");
    int times = 1; //���������ֲ�����
    do
    { //���ս����first��follow���ٱ仯������
        cout << endl;
        cout << "��" << times++ << "�α������:" << endl << endl;
        isConverge = 1;
        getFirstFollowSet();
        //VN��״̬
        for (auto it = VN2int.begin(); it != VN2int.end(); it++)
        {
            int vnindex = it->second;
            if (oldFirst[vnindex].size() != first[vnindex].size() || oldFollow[vnindex].size() != follow[vnindex].size())
            {
                isConverge = 0;
            }
            //���״̬
            cout << it->first << "��first����\t";
            for (auto first_it = first[vnindex].begin(); first_it != first[vnindex].end(); first_it++)
            {
                cout << *first_it << " ";
            }
            cout << "\t" << it->first << "��follow����\t";
            for (auto follow_it = follow[vnindex].begin(); follow_it != follow[vnindex].end(); follow_it++)
            {
                cout << *follow_it << " ";
            }
            cout << endl;
            //�����״̬���Ա�֮�����״̬�Ƚ��Ƿ�仯�ж��������
            oldFirst[vnindex] = first[vnindex];
            oldFollow[vnindex] = follow[vnindex];
        }
    } while (isConverge != 1);
}

void constructPAT()
{ //construct Predict Analyse Table,����Ԥ�������
    fill(PreAnalyseTable[0], PreAnalyseTable[0] + 50 * 50, -1);
    for (int i = 0; i < grammar.size(); i++)
    {
        int delimiterIndex = grammar[i].find("->");
        string X = grammar[i].substr(0, delimiterIndex); //��ȡÿ������ʽ���󲿷��ս��
        string rightGrama = grammar[i].substr(delimiterIndex + 2, grammar[i].length() - delimiterIndex - 2); //��ȡ�Ҳ�����ʽ

        int row = VN2int[X];
        int col;
        if (rightGrama[0] >= 'A' && rightGrama[0] <= 'Z')
        {
            string Vn = getVn(rightGrama.substr(0, 2));
            if (nullable[Vn] != true)
            {
                for (auto it = first[VN2int[Vn]].begin(); it != first[VN2int[Vn]].end(); it++)
                {
                    col = VT2int[*it];
                    PreAnalyseTable[row][col] = i;
                }
            }
            else
            {
                for (auto it = follow[VN2int[Vn]].begin(); it != follow[VN2int[Vn]].end(); it++)
                {
                    col = VT2int[*it];
                    PreAnalyseTable[row][col] = i;
                }
            }
        }
        else
        {
            string Vt = getVt(rightGrama.substr(0, rightGrama.length()));
            if (nullable[Vt] != true)
            {
                col = VT2int[Vt];
                PreAnalyseTable[row][col] = i;
            }
            else
            {
                for (auto it = follow[VN2int[X]].begin(); it != follow[VN2int[X]].end(); it++)
                {
                    col = VT2int[*it];
                    PreAnalyseTable[row][col] = i;
                }
            }
        }
    }
    //���Ԥ�������
    cout << endl<<endl;
    cout << "Ԥ����������£�" << endl;
    cout << "��������������������������������������Ԥ���������������������������������������������������" << endl;
    for (auto Vt_it = VT2int.begin(); Vt_it != VT2int.end(); Vt_it++)
    {
        int col = Vt_it->second; //�ս����Ϊ������
        cout << "\t" << Vt_it->first;
    }
    cout << endl;
    for (auto Vn_it = VN2int.begin(); Vn_it != VN2int.end(); Vn_it++)
    {
        int row = Vn_it->second; //���ս����Ϊ������
        cout << Vn_it->first << " ";
        for (auto Vt_it = VT2int.begin(); Vt_it != VT2int.end(); Vt_it++)
        {
            int col = Vt_it->second; //�ս����Ϊ������
            if (PreAnalyseTable[row][col] == -1)
                cout << "\t-";
            else
            {
                cout << "\t" << PreAnalyseTable[row][col];
            }
        }
        cout << endl;
    }
    //����ʽ���˵��
    for (int i = 0; i < grammar.size(); i++)
    {
        cout << i << ":" << grammar[i] << endl;
    }
}

int main()
{
    initGrammar();  //��ʼ���ķ�
    readVnAndVt();  //��ȡ�ķ������е�VN��VT
    converge();     //����VN��first��follow��ֱ������
    constructPAT(); //����Ԥ�������

    return 0;
}
