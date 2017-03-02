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
#include <unordered_set>

using namespace std;

class Dictionary {
public:
    bool plausible(const string&, const vector<int>&);
    
    void activate_word(string&);
    void activate_word(const string&);
    void activate_words(vector<string>&);
    void deactivate_word(string&);
    void prepare(vector<string>& words, unordered_set<string>& wordset);
    bool preserve_order;
    bool shuffle_words;
    vector<int> letter_counts(const string&);

public:
    vector<string>all_words;
    unordered_set<string> all_wordset;
    vector<string> words;
    unordered_set<string> wordset;
    
    struct {
        bool operator()(const string& a, const string& b) {
            long s = a.size() - b.size();
            if(s<0) return true;
            if(s>0) return false;
            return a.compare(b);
        }
    } cmp_shortest;
    struct {
        bool operator()(const string& a, const string& b) {
            long s = b.size() - a.size();
            if(s<0) return true;
            if(s>0) return false;
            return a.compare(b);
        }
    } cmp_longest;

  
    
    Dictionary(bool ordered=false, bool shuffled=true) : preserve_order(ordered), shuffle_words(shuffled) { }

    void load(const string&);
    void add_words(const vector<string>&);
    bool make_playable(vector<string>& words, vector<int>& unplaced, bool incremental = false);
    unsigned long size() { return words.size(); }
    bool has(const string& word) const;
    bool exists(const string& word) const;
    void dump(ostream&,bool=false);
    bool pred(int a, int b) {
        bool res = a == 0 || (a >= b);
        cout << "compare("<<a<<","<<b<<")="<<res<<endl;
        return res;
    }
    
    //TODO: fully comply with iterator requirements
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
