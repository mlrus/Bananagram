//
//  Dictionary.cpp
//  bananagram
//
//  Created by Michah Lerner on 2/9/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <functional>
#include <random>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Dictionary.h"
#include "simple_utils.h"

void Dictionary::prepare(vector<string>& words_to_prep, unordered_set<string>& wordset) {
    words_to_prep = to_upper(words_to_prep);
    if(!preserve_order) {
        cout << "Sorting words\n";
        std::sort(words_to_prep.begin(), words_to_prep.end(), cmp_shortest);
        cout << "Dedup words\n";
        auto last = std::unique(words_to_prep.begin(), words_to_prep.end());
        words_to_prep.erase(last, words_to_prep.end());
        if(shuffle_words) {
            cout  <<"Shuffle words\n";
            unsigned long long seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(words_to_prep.begin(), words_to_prep.end(), std::mt19937_64(seed));
        }
        wordset.clear();
    } else {
        cout << "Not sorting or shuffling the words\n";
    }
    wordset.insert(words_to_prep.begin(), words_to_prep.end());
}

void Dictionary::load(const string& filename) {
    rvec(filename, all_words);
    prepare(all_words, all_wordset);
}

void Dictionary::add_words(const vector<string>& data) {
    std::copy(data.begin(), data.end(), back_inserter(all_words));
    prepare(all_words, all_wordset);
}

void Dictionary::activate_word(string& word) {
    if(has(word))
        throw runtime_error("activate_Word(" + word + ") :: already active.\n");
    if(!exists(word))
        throw runtime_error("activate_Word(" + word + ") :: does not exist.\n");
    words.push_back(word);
    wordset.insert(word);
}

void Dictionary::activate_word(const string& word) {
    string uc_word(to_upper(word));
    if(has(uc_word))
        throw runtime_error("activate_Word(" + uc_word + ") :: already active.\n");
    if(!exists(uc_word))
        throw runtime_error("activate_Word(" + uc_word + ") :: does not exist.\n");
    words.push_back(uc_word);
    wordset.insert(uc_word);
}


void Dictionary::activate_words(vector<string>& words) {
    for(auto w:words) activate_word(w);
}

void Dictionary::deactivate_word(string& word) {
    if(!has(word))
        throw runtime_error("deactivate_word(" + word + ") :: no active.\n");
    if(!exists(word))
        throw runtime_error("deactivate_Word(" + word + ") :: does not exist.\n");
    auto word_at=find(words.begin(),words.end(),word);
    words.erase(word_at);
    wordset.erase(word);
}

vector<int> Dictionary::letter_counts(const string& word) {
    vector<int> counts(26,0);
    for(char ch : word) {
        if(ch<'A' || ch>'Z') throw runtime_error("Invalid character " + to_string(int(ch)) + " in word " + word);
        counts[ch-'A']++;
    }
    return counts;
}

/*
 * Add the word iff the letters peeled (and not dumped) cover the word.
 */
bool Dictionary::plausible(const string& word, const vector<int>& unplaced) {
    vector<int> counts(letter_counts(word));
    auto p = mismatch(counts.begin(), counts.end(), unplaced.begin(),
            [](int a, int b) ->bool { return (a==0 || a<=b); });
    return distance(p.first,counts.end())==0;
}

/*
 * Return true if any set bit of "nzcounts" corresponds to a char of "word".
 */
bool Dictionary::shared_ch(const string& word, const unsigned int nzmask) {
    unsigned int bitmask = charmask(word);
    return (nzmask & bitmask) != 0;
}

/* 
 * Set bit_i if ch-'A' is set.
 */
unsigned int Dictionary::charmask(const string& word) {
    unsigned int nzmask = 0;
    for(unsigned char ch : word)
        nzmask |= 0x01 << (ch - 'A');
    return nzmask;
}

/*
 * Set bit_i if count(vector[i]) is nonzero.
 */
unsigned int Dictionary::mkbinary(const vector<int>& avail) {
    unsigned int
        nzcounts = 0,
        bzero    = 0x01;
    for(auto v : avail) {
        if (v) nzcounts |= bzero;
        bzero = bzero << 1;
    }
    return nzcounts;
}

bool Dictionary::make_playable(vector<string>& words_to_enable, vector<int>& unplaced, bool incremental) {
    if(incremental) throw runtime_error("Dictionary::make_playable is unimplmented\n");
    words.clear();
    wordset.clear();
    for(auto word : words_to_enable) {
        word=to_upper(word);
        if(plausible(word,unplaced))
            words.push_back(word);
    }
    prepare(words,wordset);
    return true;
}

bool Dictionary::has(const string& word) const {
    return wordset.find(word) != wordset.end();
}
bool Dictionary::exists(const string& word) const {
    return all_wordset.find(word) != all_wordset.end();
}

void
Dictionary::dump(ostream& out, bool all) {
    for(auto w : all?all_words:words) out << w << "\n";
}

bool Dictionary::worditerator::has_next() {
    if(forward)
        return distance(it,words.end())>1;
    else
        return distance(words.begin(),it)>1;
}
const string Dictionary::worditerator::begin() {
    it = forward?words.begin()-1:words.end();
    return next();
}
const string Dictionary::worditerator::next() {
    return *(forward?(it=std::next(it)):(it=std::prev(it)));
}

