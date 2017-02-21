//
//  Place.cpp
//  bananagram
//
//  Created by Michah Lerner on 2/15/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#include "Place.h"
std::unordered_map<int, std::string> Place::PlaceNames { { Place::Direction::horizontal, "horz" },
    { Place::Direction::vertical, "vert" }};
const std::pair<int, int> Place::left(0,-1);
const std::pair<int, int> Place::right(0,1);
const std::pair<int, int> Place::up(-1,0);
const std::pair<int, int> Place::down(1,0);
