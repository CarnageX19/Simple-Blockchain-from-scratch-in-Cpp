#include<string>
#include<iostream>
#include<vector>
#include <cstdlib>
using namespace std;

string parseNext(string s)
{
    int digPtr=0;
    string prefix;
    while(s[digPtr]!='0')
    {
        prefix.push_back(s[digPtr]);
        digPtr++;
    }

    string dig;
    while(s[digPtr]=='0')
    {
        digPtr++;
    }
    for(int i=digPtr;i<s.size();i++)
    {
        dig.push_back(s[i]);
    }

    int num=stoi(dig);
    num++;

    string secHalf;
    if(num>999)
    {
        cout<<"Fatal Error: Blockchain limit reached"<<endl<<"Aborting...";
        exit(EXIT_FAILURE);
    }
    else if(num>99)
        secHalf='0'+to_string(num);
    else if(num>9)
        secHalf="00"+to_string(num);
    else
        secHalf="000"+to_string(num);
    
    return prefix+secHalf;
}
