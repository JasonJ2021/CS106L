#include <iostream>
#include <fstream>
#include <unordered_set>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ostream>
#include <iterator>
using std::cout;            using std::endl;
using std::string;          using std::unordered_set;
using std::ifstream;
/*
 * Note that you have to pass in the file as a single string
 * into the findWikiLinks function!
 * So we have to turn the passed-in file into a single string...
 * does that sound familiar at all?
 */

unordered_set<string> findWikiLinks(const string& page_html) {
    // TODO: delete this return statement and implement the
    //       function!
    unordered_set<string> result;
//    <a href="/wiki/Marine_mammal" title="Marine mammal">marine mammal</a>
    string pre = "href=";
    auto start = page_html.begin();
    auto end = page_html.end();
    while(start != end){
        auto curr = std::search(start,end,pre.begin(),pre.end());
        if(curr == end)break;
        curr+=6; // find the start;
        auto endlink = std::find(curr , end , '"');
        if(string(curr,curr+6) == "/wiki/" && std::all_of(curr+6,endlink,[](char ch){
        return ch!='#' && ch !=':';
    })){
           result.insert(string(curr+6 , endlink));
        }
        start = curr+1;
    }
    return result;
}
string fileTostring(ifstream &input){
    string result;
    string line;
    while(getline(input,line)){
        result+= line + " ";
    }
    return result;
}
int main() {
    /* Note if your file reading isn't working, please go to the
     * projects tab on the panel on the left, and in the run section,
     * uncheck the "Run in terminal" box and re-check it. This
     * should fix things.
     */

    cout << "Enter a file name: ";
    string filename;
    getline(std::cin, filename);
    string page_html;

    // TODO: Create a filestream from the filename, and convert it into
    //       a single string of data called page_html (declared above)

    // Write code here
    ifstream input(filename);
    page_html = fileTostring(input);
    cout<<page_html<<endl;
    unordered_set<string> validLinks = findWikiLinks(page_html);

    // TODO: Print out the validLinks set above.
    //       You can print it out in any format; this is just for you
    //       to be able to compare your results with the [output] results
    //       under the /res folder.

    // Write code here
    copy(validLinks.begin(),validLinks.end(),std::ostream_iterator<string>(cout , "\n"));
    cout<<"total number ="<<validLinks.size()<<endl;
    return 0;
}
