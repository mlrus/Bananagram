//
//  CharAtPos.cpp
//  Bananagram
//
//  Created by Michah Lerner on 2/26/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//


#include <ostream>
#include "CharAtPos.h"

std::ostream&
operator<<(std::ostream& out, CharAtPos& cap) {
    out << cap.dir << "@" << cap.coord;
    return out;
}

std::ostream&
operator<<(std::ostream& out, const CharAtPos& cap) {
    out << cap.dir << "@" << cap.coord;
    return out;
}
