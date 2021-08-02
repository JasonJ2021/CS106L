#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    vector<int> v = {1,24,6,8,13,3,5,10};
    sort(v.begin(),v.end(),[](int i , int j )->bool{return i >j;});
    for(auto thing : v){
        cout << thing<<endl;
    }
}