//
//  Dictionary.h
//  bananagram
//
//  Created by Michah Lerner on 2/9/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#ifndef Dictionary_h
#define Dictionary_h

#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::flush;
using std::ostream;
using std::vector;
using std::unordered_set;

class Dictionary {
    struct {
        bool operator()(const string& a, const string& b) {
            long s = a.size() - b.size();
            if(s<0) return true;
            if(s>0) return false;
            return a.compare(b);
        }
    } cmp;
    void prepare(vector<string>& words);
public:
    vector<string> words;
    unordered_set<string> wordset;
    Dictionary();
    Dictionary(const string&);
    void load(const string&);
    void add_words(const vector<string>&);
    unsigned long size() { return words.size(); }
    bool has(const string& word) const;
    virtual ~Dictionary();
    void dump(ostream&);
};

#endif /* Dictionary_h */
