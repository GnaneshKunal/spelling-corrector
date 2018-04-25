#include <iostream>
#include <regex>
#include <map>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char ** argv) {

  map<string, int> word;

  word["ABC"] = 1;
  word["abc"] = 2;

  cout << word["ab"] << endl;

  regex exp("(\\b\\S*\\b)");
  smatch res;
  string str = "first second third forth";

  while (regex_search(str, res, exp)) {
    cout << res[0] << endl;
    str = res.suffix();
  }
  
  return 0;
}
