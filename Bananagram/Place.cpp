//
//  Place.cpp
//  bananagram
//
//  Created by Michah Lerner on 2/15/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "Place.h"

std::unordered_map<int, std::string> Place::PlaceNames { { Place::Direction::horizontal, "horz" },
    { Place::Direction::vertical, "vert" }};

const Coord Place::left(0,-1);
const Coord Place::right(0,1);
const Coord Place::up(-1,0);
const Coord Place::down(1,0);

std::ostream&
operator<<(std::ostream& out, const Place& place) {
    out << place.coord << ":" << Place::PlaceNames[place.dir];
    return out;
}
