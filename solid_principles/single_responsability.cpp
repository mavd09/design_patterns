/*
 * Single Responsability Principle
 * 
 * Every class in a computer program should have responsability
 * over a single part of that program's functionality, which it
 * should encapsulate.
 */

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

struct Journal {
  string title;
  vector<string> entries;

  Journal(const string& title) : title(title) {}
  
  void add_entry(const string& entry) {
    static int count = 1;
    entries.push_back(lexical_cast<string>(count++) + ": " + entry);
  }

  // In this case, we should avoid this method inside this struct
  // because Journal is not resposible to persist data
  void save(const string& filename) {
    ofstream ofs(filename);
    for(auto& e : entries) {
      ofs << e << endl;
    }
  }
};

struct PersistenceManager {
  static void save(const Journal& journal, const string& filename) {
    ofstream ofs(filename);
    for(auto& e : journal.entries) {
      ofs << e << endl;
    }
  }
};

int main() {
  Journal journal{"Dear diary"};

  journal.add_entry("I ate a bug");
  journal.add_entry("I cried today");

  // journal.save("diary.txt");
  PersistenceManager pm;
  pm.save(journal, "diary.txt");

  return 0;
}

