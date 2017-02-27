//
//  Coord.cpp
//  Bananagram
//
//  Created by Michah Lerner on 2/26/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#include <ostream>
#include "Coord.h"

std::ostream&
operator<<(std::ostream& out, Coord& coord) {
    out << "(" << coord.row << "," << coord.col << ")";
    return out;
}

std::ostream&
operator<<(std::ostream& out, const Coord& coord) {
    out << "(" << coord.row << "," << coord.col << ")";
    return out;
}
