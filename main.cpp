//
//  main.cpp
//  LL(1)
//
//  Created by Friedhelm Liu on 2021/5/10.
//

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <cstring>
#include <set>

using namespace std;

map <char, int> getnum;
char getChar[100];
vector <string> proce;
int table[100][100];
int num = 0;
int numvt = 0;

string first[100];
string follow[200];

void readin()
{
    memset(table, -1, sizeof(table));
    getnum['#'] = 0;
    getChar[0] = '#';
    cout << "Please input all the terminals: " << endl;
    char x;
    do
    {
        cin >> x;
        getnum[x] = ++num;
        getChar[num] = x;
    }while(cin.peek()!='\n');
    numvt = ++num;
    getnum['@'] = numvt;
    getChar[num] = ('@');
    
    cout << "Please input all the nonterminals: " << endl;
    do
    {
        cin >> x;
        getnum[x] = ++num;
        getChar[num] = x;
    }while(cin.peek()!='\n');
    cout << "Input set of productions(use '@' to instead of ∊), end with 'end':" << endl;
    string pro;
    while(cin >> pro && pro != "end")
    {
        string ss;
        ss += pro[0];
        for(int i=3; i<pro.size(); i++)
        {
            if(pro[i] == '|')
            {
                proce.push_back(ss);
                ss.clear();
                ss += pro[0];
            }
            else
            {
                ss += pro[i];
            }
        }
        proce.push_back(ss);
    }
}

void intersection(string &a, string b)        //取交集
{
    set <char> se;
    for (int i = 0; i < a.size(); i++)
    {
        se.insert(a[i]);
    }
    for (int i = 0; i < b.size(); i++)
    {
        se.insert(b[i]);
    }
    string ans;
    set <char> :: iterator it;
    for (it = se.begin(); it != se.end(); it++)
    {
        ans += *it;
    }
    a = ans;
}

string get_f(int vn, int & has_0)
{
    if(vn == numvt)
        has_0 = 1;
    if(vn < numvt)
        return first[vn];
    string ans;
    for(int i = 0; i < proce.size(); i++)
    {
            if(getnum[proce[i][0]] == vn)
                ans += get_f(getnum[proce[i][1]], has_0);
    }
    return ans;
}

void firstset()
{
    for(int i = 1; i <= numvt; i++)
    {
        first[i] += ('0' + i);
        
    }
    for(int j = 0; j < proce.size(); j++)
    {
        int k = 0;
        int has_0 = 0;
        do
        {
            has_0 = 0;
            k++;
            if(k == proce[j].size())
            {
                first[getnum[proce[j][0]]] += ('0' + numvt);
                break;
            }
            intersection(first[getnum[proce[j][0]]], get_f(getnum[proce[j][k]], has_0));
        }while(has_0);
    }
}

void printfirst()
{
    cout << "First Set: " << endl;
    for(int i = 1; i<= num; i++)
    {
        cout << "First [" << getChar[i] << "]: ";
        for(int j = 0; j < first[i].size(); j++)
        {
            cout << getChar[first[i][j] - '0'] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void followset()
{
    intersection(follow[getnum[proce[0][0]]], "0");
    for(int j = 0; j < proce.size(); j++)
    {
        for(int k = 1; k < proce[j].size(); k++)
        {
            if(getnum[proce[j][k]] <= numvt)
                continue;
            int n = k;
            int has_0;
            do
            {
                has_0 = 0;
                n++;
                if(n == proce[j].size())
                {
                    intersection(follow[getnum[proce[j][k]]], follow[getnum[proce[j][0]]]);
                    break;
                }
                intersection(follow[getnum[proce[j][k]]], get_f(getnum[proce[j][n]], has_0));
            }while(has_0);
        }
    }
}

void gettable()
{
    for(int i = 0; i < proce.size(); i++)
    {
        if(proce[i][1] == '@')
        {
            string flw = follow[getnum[proce[i][0]]];
            for(int k = 0; k < flw.size(); k++)
            {
                table[getnum[proce[i][0]]][flw[k]-'0'] = i;
            }
        }
        string temps = first[getnum[proce[i][1]]];
        for(int j = 0; j < temps.size(); j++)
        {
            if(temps[j] != ('0' + numvt))
            {
                table[getnum[proce[i][0]]][temps[j] - '0'] = i;
            }
            else
            {
                string flw = follow[getnum[proce[i][1]]];
                for(int k = 0; k < flw.size(); k++)
                {
                    table[getnum[proce[i][0]]][flw[k] - '0'] = i;
                }
            }
        }
    }
}

string get_proce(int i)
{
    if(i < 0)
        return " ";
    string ans;
    ans += proce[i][0];
    ans += "->";
    for(int j = 1; j < proce[i].size(); j++)
        ans += proce[i][j];
    return ans;
}

void print_table()
{
    cout << "Predictive table: " << endl;
    for(int i = 0; i < numvt; i++)
    {
        cout << '\t' << getChar[i];
    }
    cout << endl;
    for(int i = numvt+1; i<=num; i++)
    {
        cout << getChar[i];
        for(int j = 0; j < numvt; j++)
        {
            cout << '\t' << get_proce(table[i][j]);
        }
        cout << endl;
    }
    cout << endl;
}

void printFollow()
{
    cout << "Follow Set: " << endl;
    for(int i = numvt+1; i <= num; i++)
    {
        cout << "Follow [" << getChar[i] << "]: ";
        for(int j = 0; j < follow[i].size(); j++)
        {
            cout << getChar[follow[i][j] - '0'] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

string word;

bool analyze()
{
    stack <char> sta;
    sta.push('#');
    sta.push(proce[0][0]);
    int i = 0;
    while(!sta.empty())
    {
        int cur = sta.top();
        sta.pop();
        if(cur == word[i])
        {
            i++;
        }
        else if(cur == '#')
        {
            return 1;
        }
        else if(table[getnum[cur]][getnum[word[i]]] != -1)
        {
            int k = table[getnum[cur]][getnum[word[i]]];
            cout << proce[k][0] << "->";
            for(int j = 1; j < proce[k].size(); j++)
            {
                cout << proce[k][j];
            }
            cout << endl;
            for(int j = proce[k].size()-1; j > 0; j--)
            {
                if(proce[k][j] != '@')
                    sta.push(proce[k][j]);
            }
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    readin();
    firstset();
    followset();
    followset();
    gettable();
    printfirst();
    printFollow();
    print_table();
    cout << "Please enter expression: " << endl;
    cin >> word;
    if(analyze())
        cout << "Succeed! Production used as above." << endl;
    else
        cout << "Error!" << endl;
    return 0;
}
