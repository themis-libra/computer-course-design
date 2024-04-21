#include"stdc++.h"

using namespace std;


ifstream infile("test.sy");
ofstream outfile("result.txt");

set<string> Key = { "break","struct","const","enum","typedef","if","else","sizeof","void","continue","int","char" };
set<string> Op = { "+","-","*","/","%","==","!=","<",">","<=",">=","!","&&","||","|","&","!","^",".","->","=","~" };
set<string> Delimiter = { ",",";","[","]","(",")","{","}" };
set<string> Comment = { "//","/*","*/" };

map <string, int> dic;//构造映射 

void init()
{
    for (auto p : Key)  dic[p] = 1;   //映射类别号
    for (auto p : Op)   dic[p] = 2;
    for (auto p : Delimiter) dic[p] = 3;

    string s = "";
    for (char ch = '0'; ch <= '9'; ch++) 
    {
        s.push_back(ch);
        dic[s] = 4;
        s.pop_back();
    }
    for (char ch = 'a'; ch <= 'z'; ch++) 
    {
        s.push_back(ch);
        dic[s] = 4;
        s.pop_back();
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) 
    {
        s.push_back(ch);
        dic[s] = 4;
        s.pop_back();
    }
    dic["_"] = 4;

    return;
}
bool check_Key(string s) //判断是不是关键字
{ 
    return Key.count(s) ? true : false;
}
bool check_Id(string s) //标识符是否符合规则
{ 
    regex pattern("^[A-Za-z_][0-9A-Za-z_]*$");
    return regex_match(s, pattern);
}
bool check_Op(string s) 
{
    return Op.count(s) ? true : false;
}
bool check_Delimiter(string s) 
{
    return Delimiter.count(s) ? true : false;
}
bool check_Num(string s, int& p, int& ne) 
{
    int i = 0;
    ne = 1;
    if (s[0] == '-') 
    {
        ne = -1;
        if (s.size() == 1)return false;
        s.substr(1);
    }
    if (s[0] == '0') 
    {
        i++;
        p = 8;
        if (s.size() == 1) 
        {
            p = 10;
            return true;
        }
        else if (s[1] == 'x' || s[i] == 'X') 
        {
            p = 16; i++;
            if (s.size() == 2)return false;
        }
    }
    while (i < s.size() && s[i] == '0') 
    {
        i++; continue;
    }
    while (i < s.size() && (isdigit(s[i]) || (p == 16 && (s[i] >= 'a' && s[i] <= 'f' || s[i] >= 'A' && s[i] <= 'F')))) 
    {
        i++; continue;
    }
    if (i < s.size())return false;
    return true;
}
int Cal_Num(string s, int p, int ne) 
{
    int co = 0;
    if (ne == -1)co++;
    if (p == 8)co++;
    else if (p == 16)co += 2;
    s = s.substr(co);
    int ans = 0;
    for (int i = 0; i < s.size(); i++) 
    {
        if (isdigit(s[i]))
            ans = ans * p + s[i] - '0';
        else ans = ans * p + s[i] - 'a' + 10;
    }
    return ans * ne;
}
bool check_Comment(string s) 
{
    return Comment.count(s) ? true : false;
}
void Error_one(int row) 
{
    outfile << "Error [unrecognized] at line: [" << row << "]: Do not find a matched type" << endl;
}
void Error_two(int row) 
{
    outfile << "Error [unmatched] at line: [" << row << "]: without '*/' to match '/*'" << endl;
}
void Error_three(int row) 
{
    outfile << "Error [unmatched] at line: [" << row << "]: without '/*' to match '*/'" << endl;
}

int main()
{
    init();


    string word1, str;//获取单词word

    char ch;
    ostringstream buf;
    while (buf && infile.get(ch))
        buf.put(ch);
    str = buf.str();

    int len = (int)str.size();
    int row = 1;
    for (int i = 0; i < len; i++) //遍历str
    {
        word1 = "";
        int p = 10, ne = 1;
        while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') //去除空格、换行符
        {
            if (str[i] == '\n')row++;
            i++;
        }
        while (i < len && str[i] != ' ' && str[i] != '\n' && str[i] != '\t') //切分单词
        {
            string s1 = "", s2 = "";
            s1 += str[i];
            if (word1.size() == 0 && dic[s1] == 3) //分隔符
            {
                word1 += str[i];
                i++; break;
            }
            if (word1.size() >= 1) 
            {
                s2 += str[i - 1];
                if (dic[s1] != dic[s2]) 
                {
                    break;
                }
                if (dic[s1] == 2) 
                {
                    word1 += str[i];
                    if (check_Comment(word1))break;
                    if (!check_Op(word1)) 
                    {
                        word1.pop_back();
                        break;
                    }
                    i++; break;
                }
            }
            word1 += str[i]; i++;
        }
        if (word1 == "")//到文本结尾
            break;
        if (check_Key(word1)) 
        {
            outfile << "< " << word1 << " >" << endl;
        }
        else if (check_Id(word1)) 
        {
            outfile << "< ID , \"" << word1 << "\" >" << endl;
        }
        else if (check_Op(word1)) 
        {
            outfile << "< OP , " << word1 << " >" << endl;
        }
        else if (check_Comment(word1)) 
        {
            if (word1 == "//")
                while (i < len && str[i] != '\n')
                    i++;
            else if (word1 == "/*") 
            {
                i++;
                while (i < len && (str[i - 1] != '*' || str[i] != '/')) 
                {
                    if (str[i - 1] == '\n')row++;
                    i++;
                }
                if (i == len) 
                {
                    Error_two(row);
                }
                i++;
            }
            else if (word1 == "*/") 
            {
                Error_three(row);
            }
            if (word1 == "//") 
            {
                outfile << "< Comment , " << word1 << " >" << endl;
            }
            else 
            {
                outfile << "< Comment , /*..*/ >" << endl;
            }
        }
        else if (check_Delimiter(word1)) 
        {
            outfile << "< Delimiter , " << word1 << " >" << endl;
        }
        else if (check_Num(word1, p, ne)) 
        {
            int val = Cal_Num(word1, p, ne);
            outfile << "< NUM , " << val << " >" << endl;
        }
        else if (check_Num(word1, p, ne) == false)
        {
            outfile << "Error [unrecognized] at line: [" << row << "]: "<< word1 <<  " error" << endl;
        }
        else 
        {
            Error_one(row);
        }
        i--;
    }
    infile.close();
    outfile.close();
    return 0;
}