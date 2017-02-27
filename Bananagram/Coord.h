//
//  Coord.hpp
//  Bananagram
//
//  Created by Michah Lerner on 2/26/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#ifndef Coord_h
#define Coord_h

#include <algorithm>

class Coord {
public:
    int row;
    int col;
    Coord(int a, int b) : row(a), col(b) { }
    Coord(unsigned long a, unsigned long b) : row(int(a)), col(int(b)) { }
    Coord& operator=(Coord arg) {
        row = arg.row;
        col = arg.col;
        return *this;
    }
    Coord operator+(const Coord& other) const {
        return Coord(row+other.row,col+other.col);
    }
    Coord operator+(Coord& other) const {
        return Coord(row+other.row,col+other.col);
    }
    Coord operator-(const Coord& other) const {
        return Coord(row-other.row,col-other.col);
    }
    Coord operator-(Coord& other) const {
        return Coord(row-other.row,col-other.col);
    }
    Coord& operator+=(const Coord& other) {
        row+=other.row;
        col+=other.col;
        return *this;
    }
    Coord& operator-=(const Coord& other) {
        row-=other.row;
        col-=other.col;
        return *this;
    }
    bool operator<(const Coord& other) {
        return std::tie(row,col) < std::tie(other.row,other.col);
    }
    bool operator==(const Coord& other) {
        return std::tie(row,col) == std::tie(other.row,other.col);
    }
    bool operator>(const Coord& other) {
        return std::tie(row,col) > std::tie(other.row,other.col);
    }
};

std::ostream& operator<<(std::ostream& out, const Coord& place);

#endif /* Coord_h */
