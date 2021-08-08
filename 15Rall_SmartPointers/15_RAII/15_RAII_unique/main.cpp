#include <iostream>
#include <string>
#include <vector>
#include "uniqueptr.h"

using std::cout;    using std::endl;
using std::cin;     using std::string;
using std::vector;

void rawPtrFn() {
    int* n = new int(5);
    ++*n;

    cout << "=> " << *n << endl;
    delete n;
}

void smartPtrFn() {
    UniquePtr<int> n(new int(10));
    ++*n;
    cout << "=> " << *n << endl;

    cout << "=> after " << *n << endl;
}



int main() {
    rawPtrFn();
    smartPtrFn();





    return 0;
}
