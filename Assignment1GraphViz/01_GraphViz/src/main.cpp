#include <iostream>
#include "SimpleGraph.h"
#include <string>
#include <string.h>
using namespace std;
void Welcome();

// Main method
int main() {
    Welcome();
    return 0;
}
/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
    cout << "Please type the name of the graph"<< endl;
    SimpleGraph graph;
    string filename;
    getline(cin,filename);
    cout<<filename<<endl;

    readGraph(filename.c_str(),graph);
    positionInit(graph);
    InitGraphVisualizer(graph);
    DrawGraph(graph);
    while(true){
        computeForce(graph);
        DrawGraph(graph);
    }
}
