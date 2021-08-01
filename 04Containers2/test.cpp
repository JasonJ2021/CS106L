#include <iostream>
#include <set>

using namespace std;

int main()
{
    set<int> myset;
    myset.insert(1);
    myset.insert(2);
    myset.insert(3);
    myset.insert(4);
    set<int>::iterator iter = myset.begin();
    while(iter != myset.end()){
        cout<<*iter<<endl;
        iter++;
    }
}