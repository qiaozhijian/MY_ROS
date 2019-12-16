#include "utility.h"

using namespace std;

void printPath()
{
    char   buffer[100];   
    getcwd(buffer, 100);   
    cout<<"The current directory is: "<<buffer<<endl;
}
string zfill(string str,int num)
{
    string out_str=str;
    if(out_str.empty())
        out_str="0";
    for(int i=0;i<num;i++)
    {
        if(str.size()<i+1)
        {
            out_str="0" + out_str;
        }else{
            
        }
        
    }
    return out_str;
}
