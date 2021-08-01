#include <iostream>
#include "SimpleGraph.h"
#include <string>
#include <sstream>
#include <fstream>
using std::cout;	using std::endl;
using std::cin;
using std::string;
void Welcome();

// Main method
int main() {
    Welcome();
    return 0;
}
std::string Getline();
int GetInteger();
/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
    cout << "Please type the name of the graph"<<endl;
    SimpleGraph graph = getGraph();
    positionInit(graph);
    InitGraphVisualizer(graph);
    DrawGraph(graph);
    while(true){
        computeForce(graph);
        DrawGraph(graph);
    }
}
int getInteger() {
    while(true) {
        /* First we get a line of input from the user. */
        string line;
        std::getline(cin, line);
        /*
         * We'll again use an istringstream because we only want to pull data
         * out of the stream once we have put the string's contents in it.
         */
        std::istringstream converter(line);
        /*
         * Try getting an int from the stream. If this is not succesful
         * then user input was not a valid input.
         */
        int result;
        if(converter >> result) {
            /*
             * See if we can extract a char from the stream.
             * If so, the user had junk after a valid int in their input.
             */
            char remaining;
            if(converter >> remaining) {
                cout << "Unexpected character. Try again." << endl;
            } else {
                /*
                 * Input was succesfully converted to int with no
                 * trailing stuff at the end.
                 */
                return result;
            }
        } else {
            cout << "Not a valid integer. Try again." << endl;
        }
    }
}
std::string Getline(){
    std::string result;
    getline(std::cin,result);
    return result;
}
int GetInteger(){
    int result;
    result = getInteger();
    return result;
}
