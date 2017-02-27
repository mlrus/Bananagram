//
//  simple_utils.h
//  simi
//
//  Created by Michah Lerner on 12/29/13.
//  Copyright (c) 2013 Michah Lerner. All rights reserved.
//

#ifndef __simple_utils__
#define __simple_utils__

#include <iostream>
#include <sstream>
#include <iterator>
#include <list>
#include <vector>
#include <deque>
#include <string>
#include <unordered_map>

#include "defs.h"
#include "Place.h"
#include "Coord.h"
#include "CharAtPos.h"

using std::string;

template <template<typename, typename> class Container, typename Value, typename Allocator=std::allocator<Value>>
std::ostream& operator<<(std::ostream& out, const Container<Value, Allocator>& vec) {
    for(typename Container<Value,Allocator>::const_iterator it = vec.cbegin();
        it != vec.cend(); it++) { out << *it << "; "; };
    return out;
}

//std::ostream& operator<<(std::ostream& out, Coord& place);
//std::ostream& operator<<(std::ostream& out, const Place& place);
//std::ostream& operator<<(std::ostream& out, const Coord& place);
//std::ostream& operator<<(std::ostream& out, const CharAtPos& c_a_p);
std::ostream& operator<<(std::ostream& out, const vector<char>& vec);
std::ostream& operator<<(std::ostream& out, const vector<vector<char>>& vec);

void
showargs(int argc, char * const argv[]);

vector<char>
initialize_tiles();

void
shuffle_tiles(vector<char>& tiles);

vector<char>
tochar(const string& str);

int
fexist(const string & name);

off_t
fsize(const string & name);

off_t
fsize(int fd);

string
unique_name(const string & base);

template <template <typename,typename> class C, typename E, typename A>
void
rvec(const string & name, C<E,A> &dat);

template <class T>
string
tobin(T v);

string&
toupper(std::string &s);

void
upperCase(string & in);

void
trim(string& in);

string
tod();

string
getBasename(const string& path, const string& suffix="");

string
getDirname(const string& path);

string
getCwd();

#endif
