//
//  CharAtPos.hpp
//  Bananagram
//
//  Created by Michah Lerner on 2/26/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#ifndef CharAtPos_h
#define CharAtPos_h

#include "Coord.h"

class CharAtPos {
public:
    char dir;
    Coord coord;
    CharAtPos(const CharAtPos& other) : dir(other.dir), coord(other.coord) { }
    CharAtPos(char a, int b, int c): dir(a), coord(Coord(b,c)) { }
    CharAtPos(char d, Coord c) : dir(d), coord(c) { }
};

std::ostream& operator<<(std::ostream& out, const CharAtPos& cap);

#endif /* CharAtPos_h */
