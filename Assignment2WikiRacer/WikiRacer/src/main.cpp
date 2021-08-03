#include <iostream>     // for cout, cin
#include <fstream>      // for ifstream
#include <sstream>      // for stringstream
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include "wikiscraper.h"
#include <utility>
#include <set>
#include <algorithm>

using std::cout;            using std::endl;
using std::ifstream;        using std::stringstream;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;   using std::cin;

/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */
vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    // TODO: 1. Once your file reading is working, replace the below
    //          return statement with "return {};".
    //       2. Make sure that you've copied over your findWikiLinks method
    //          from Part A into wikiscraper.cpp.
    //       3. Finally, implement this function per Part B in the handout!
    //
    //                Best of luck!
    WikiScraper scraper;
    auto endlink = scraper.getLinkSet(end_page);
    unordered_set<string> hasVisited;
    auto cmpfn = [&](vector<string> ladder1 , vector<string> ladder2){
      string page1 = ladder1[ladder1.size()-1];
      string page2 = ladder2[ladder2.size()-1];
      auto set1 = scraper.getLinkSet(page1);
      auto set2 = scraper.getLinkSet(page2);
      int num1 = 0;
      int num2 = 0;
      for(auto thing :set1){
          if(endlink.find(thing) != endlink.end()){
              num1++;
          }
      }
      for(auto thing :set2){
          if(endlink.find(thing) != endlink.end()){
              num2++;
          }
      }
      return num1<num2;
    };
    std::priority_queue<vector<string>,vector<vector<string>>,decltype (cmpfn)>ladderQueue(cmpfn);
    vector<string> starter = {start_page};
    ladderQueue.push(starter);
    hasVisited.insert(start_page);
    while(!ladderQueue.empty()){
        vector<string> temp = ladderQueue.top(); //c++pop() has no return value
        ladderQueue.pop();
        auto links = scraper.getLinkSet(temp[temp.size()-1]);
        if(links.find(end_page) != links.end()){
                temp.push_back(end_page);
                return temp;
        }
        for(auto thing : links){
            if(hasVisited.find(thing) == hasVisited.end()){
                hasVisited.insert(thing);
                vector<string> copyvec = temp;
                copyvec.push_back(thing);
                ladderQueue.push(copyvec);
            }
        }
    }
    return {};
}

int main() {
    /* Container to store the found ladders in */
    vector<vector<string>> outputLadders;

    cout << "Enter a file name: ";
    string filename;
    getline(cin, filename);

    // TODO: Create a filestream from the filename.
    //       For each pair {start_page, end_page} in the input file,
    //       retrieve the result of findWikiLadder(start_page, end_page)
    //       and append that vector to outputLadders.

    // Write code here
    ifstream fileopen(filename);
    int pairs;
    fileopen>>pairs;
    cout<<pairs;
    for(int i = 0 ;i<pairs;i++){
        string start_page ,end_page;
        fileopen>>start_page>>end_page;
        outputLadders.push_back(findWikiLadder(start_page,end_page));
    }

//     * Print out all ladders in outputLadders.
//     * We've already implemented this for you!
//     */
    for (auto& ladder : outputLadders) {
        if(ladder.empty()) {
            cout << "No ladder found!" << endl;
        } else {
            cout << "Ladder found:" << endl;
            cout << "\t" << "{";

            std::copy(ladder.begin(), ladder.end() - 1,
                      std::ostream_iterator<string>(cout, ", "));
            /*
             * The above is an alternate way to print to cout using the
             * STL algorithms library and iterators. This is equivalent to:
             *    for (size_t i = 0; i < ladder.size() - 1; ++i) {
             *        cout << ladder[i] << ", ";
             *    }
             */
            cout << ladder.back() << "}" << endl;
        }
    }
    return 0;
}




