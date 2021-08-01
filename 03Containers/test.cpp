#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<algorithm>
#include<map>
using namespace std;

int getInteger(){
    string line;
    getline(cin,line);
    istringstream converter(line);
    int result;
    if(converter >> result){
        char remain;
        if(converter >>remain){
            cout<<"budui"<<endl;
        }else{
            return result;
        }
    }else{
        cout<<"sth gets wrong"<<endl;
    }
    return 0;
}
void printVector(const vector<int>& data){
    for(int i = 0 ; i < data.size() ;i++){
        cout<< data.at(i) << " ";
    }
    cout<<endl;
}
int main(){
    const int kNumValues = 5;
    vector<int> data;
    for(int i = 0 ; i < kNumValues ; i++){
        cout<<"Enter value (" << kNumValues - i <<"Left): ";
        int cur = getInteger();
        data.push_back(cur);

    }
    int size = data.size();
    if(size == 0 ){
        cout<<"No data entered."<<endl;
        return 1;
    }
    sort(data.begin() , data.end());
    int mid = size /2;
    double median;
    if(size % 2){
        median = data[mid];
    }else{
        median = (data[mid-1]+ data[mid])/2.0;
    }
    printVector(data);
    map<string , int> m;
    m.insert("孙加辉","11");
    cout<< "The median is "<< median<<endl;
}