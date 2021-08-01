#include<iostream>
#include<string>
#include<iomanip>
#include<sstream>
using namespace std;

int main(){
    string line = "137 2.718 Hello";
    stringstream stream(line);
    int myInt;
    double mydouble;
    string mystring;
    stream>>myInt>>mydouble>>mystring;
    cout<<myInt<<endl;
    cout<<mydouble<<endl;
    cout<<mystring<<endl;
}