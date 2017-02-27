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
    bool preserve_order;
    bool shuffle_words;
public:
    vector<string> words;
    unordered_set<string> wordset;
    Dictionary(bool ordered=false, bool shuffled=true) : preserve_order(ordered), shuffle_words(shuffled) {}
    virtual ~Dictionary();
    void load(const string&);
    void add_words(const vector<string>&);
    unsigned long size() { return words.size(); }
    bool has(const string& word) const;
    void dump(ostream&);
    
    
    class worditerator {
        const vector<string>& words;
        vector<string>::const_iterator it;
        bool forward = true;
    public:
        const string begin();
        const string next();
        bool has_next();
        worditerator(const vector<string>& w, bool dir=true) : words(w), forward(dir) {
            it = forward?words.begin()-1:words.end();
        }
    };
};

#endif /* Dictionary_h */
