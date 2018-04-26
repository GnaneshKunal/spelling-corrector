#include <iostream>
#include <regex>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <set>
#include <tuple>
#include <iterator>

using namespace std;

void insert_to_map_set(map<string, int> &, set<string> &, string);
long int sum_values(map<string, int> &);
inline double P(map<string, int> &, string, long int);
void insert_to_map_set(map<string, int> &, set<string> &, string);
long int sum_values(map<string, int> &);
set<string> edits1(string);
set<string> edits2(string);
set<string> known(map<string, int> &, set<string>);
set<string> candidates(map<string, int> &, string);
string correction(map<string, int> &, string);

int main(int argc, char ** argv) {

  
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " [bigfile] [word]" << endl;
    exit(0);
  }
  
  map<string, int> words;
  set<string> u_words;

  
  regex exp("(\\b\\S*\\b)");
  smatch res;
  
  
  ifstream infile(argv[1]);
  string line;
  while (getline(infile, line)) {
    while (regex_search(line, res, exp)) {
      string word = res[0];
      transform(word.begin(), word.end(), word.begin(), ::tolower);
      insert_to_map_set(words, u_words, word);
      line = res.suffix();
    }
  }

  long int N = sum_values(words);

  cout << correction(words, argv[2]);
  
  return 0;
}

void insert_to_map_set(map<string, int> & words, set<string> & u_words, string word) {
  if (words[word] == 0){
    words[word] = 1;
    u_words.insert(word);
  }
  else
    words[word] += 1;
}

long int sum_values(map<string, int> & words) {
  int sum = 0;
  
  for (map<string, int>::iterator it = words.begin(); it != words.end(); ++it) {
    sum += words[it->first];
  }

  return sum;
}


inline double P(map<string, int> & words, string word, long int N) {
  return (double)words[word] / N;
}

set<string> edits1(string word) {
  string letters = "abcdefghijklmnopqrstuvwxyz";

  vector<tuple<string, string>> splits;
  for (int i = 0; i < word.length(); i++)
    splits.push_back(make_tuple(word.substr(0, i), word.substr(i, -1)));

  set<string> return_set;

  for (auto& t: splits) {
    if (get<0>(t).compare("")) {
      if ((get<1>(t)).length() > 1)
	return_set.insert(get<0>(t) + get<1>(t)[1] + get<1>(t)[0] + (get<1>(t)).substr(2, -1)); // transposes
      return_set.insert(get<0>(t) + (get<1>(t)).substr(1, -1)); // deletes
      for (auto it = letters.begin(); it < letters.end(); ++it)
	return_set.insert(get<0>(t) + *it + (get<1>(t)).substr(1, -1)); // replaces
    }
    for (auto it = letters.begin(); it < letters.end(); ++it)
      return_set.insert(get<0>(t) + *it + get<1>(t)); // inserts
  }
  
  return return_set;
}


set<string> edits2(string word) {

  set<string> return_set;
  
  for (auto& e1 : edits1(word)) {
    for (auto& e2: edits1(e1)) {
      return_set.insert(e2);
    }
  }

  return return_set;
}

set<string> known(map<string, int> & words, set<string> my_words) {

  set<string> return_set;

  for (auto& w: my_words) {
    if (words[w] != 0) {
      return_set.insert(w);
    }
  }

  return return_set;
}

set<string> candidates(map<string, int> & words, string word) {

  set<string> return_set;
  return_set = known(words, set<string> { word });
  if (return_set.size() != 0)
    return return_set;
  
  return_set = known(words, edits1(word));
  if (return_set.size() != 0)
    return return_set;

  return_set = known(words, edits2(word));
  if (return_set.size() != 0)
    return return_set;

  return set<string> { word };
}


string correction(map<string, int> & words, string word) {

  vector<double> scores;
  long int sum = sum_values(words);
  set<string> get_candidates = candidates(words, word);

  for (auto& gc: get_candidates) {
    scores.push_back(P(words, gc, sum));
  }

  int n = distance(scores.begin(), max_element(scores.begin(), scores.end()));

  auto it = get_candidates.begin();
  advance(it, n);
  return *it;
}

