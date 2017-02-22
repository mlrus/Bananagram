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

Dictionary::Dictionary() : words(), wordset() {}

Dictionary::Dictionary(const string& filename) : words() {
    load(filename);
}

void Dictionary::prepare(vector<string>& words) {
    std::transform(words.begin(), words.end(), words.begin(),
                   [](string& s) { return toupper(s); });
    std::sort(words.begin(), words.end(), cmp);
    auto last = std::unique(words.begin(), words.end());
    words.erase(last, words.end());
    unsigned long long seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(words.begin(), words.end(), std::mt19937_64(seed));
    wordset.clear();
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

