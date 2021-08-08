#include <iostream>
#include <string>
#include <vector>
#include "sharedptr.h"

using std::cout;    using std::endl;
using std::cin;     using std::string;
using std::vector;

void rawPtrFn() {
    int* n = new int(5);
    for(int i = 0; i < 20; ++i) {
        if(i == 3) {
            cout << "=> " << *n << endl;
            return;
        }
    }
    delete n;
}

void smartPtrFn() {
    int *p = new int(10);
    SharedPtr<int> n(p);

    SharedPtr<int> n2(p);
    cout << *n << endl;
    ++*n2;
    cout << *n << endl;
}


int main() {
    rawPtrFn();
    smartPtrFn();

    return 0;
}
