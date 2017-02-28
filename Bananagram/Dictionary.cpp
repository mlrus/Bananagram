//
//  Dictionary.cpp
//  bananagram
//
//  Created by Michah Lerner on 2/9/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#include "Dictionary.h"
#include "simple_utils.h"
#include <chrono>
#include <random>

void Dictionary::prepare(vector<string>& words) {
    cout << "Ucase words\n";
    std::transform(words.begin(), words.end(), words.begin(),
                   [](string& s) { return toupper(s); });
    if(!preserve_order) {
        cout << "Sorting words\n";
        std::sort(words.begin(), words.end(), cmp_shortest);
        cout << "Dedup words\n";
        auto last = std::unique(words.begin(), words.end());
        words.erase(last, words.end());
        if(shuffle_words) {
            cout  <<"Shuffle words\n";
            unsigned long long seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(words.begin(), words.end(), std::mt19937_64(seed));
        }
        wordset.clear();
    } else {
        cout << "Not sorting or shuffling the words\n";
    }
    wordset.insert(words.begin(), words.end());
}

void Dictionary::load(const string& filename) {
    rvec(filename, words);
    prepare(words);
}

void Dictionary::add_words(const vector<string>& data) {
    std::copy(data.begin(), data.end(), back_inserter(words));
    prepare(words);
}

Dictionary::~Dictionary() {
    words.clear();
}

bool Dictionary::has(const string& word) const {
    return wordset.find(word) != wordset.end();
}

void
Dictionary::dump(ostream& out) {
    for(auto w : words) out << w << "\n";
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

