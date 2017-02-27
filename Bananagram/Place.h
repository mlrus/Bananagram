//
//  Place.hpp
//  bananagram
//
//  Created by Michah Lerner on 2/15/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#ifndef Place_h
#define Place_h

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <algorithm>

//#include "defs.h"

#include "Coord.h"

using namespace std;

class Place {
public:
    enum Direction { horizontal, vertical };
public:
    Coord coord;
    Direction dir;
public:
    static std::unordered_map<int, std::string> PlaceNames;
    
    static const Coord right, left, up, down;
    Place(int r, int c, Direction dir) : coord(Coord(r,c)), dir(dir) { }
    Place(const Coord& c, Direction d) : coord(c), dir(d) { }
    Place(Place &pl) : coord(pl.coord), dir(pl.dir) { }
    Place(const Place &pl) : coord(pl.coord), dir(pl.dir) { }
    Place(int r, int c, char d) : coord(r,c), dir(d=='r'?Direction::horizontal:Direction::vertical) {
        if(d!='r'&&d!='c') throw std::out_of_range("dir_error");
    }
    virtual ~Place() { }
    
    //Place& operator=(Place arg) { std::swap(*this,arg); return *this; }
    Place& operator=(Place arg) {
        coord = arg.coord;
        dir = arg.dir;
        return *this;
    }
    Place& operator++() { // pre-increment
        if(dir == Direction::horizontal)
            coord.col++;
        else
            coord.row++;
        return *this;
    }
    Place& operator--() { // pre-increment
        if(dir == Direction::horizontal)
            coord.col--;
        else
            coord.row--;
        return *this;
    }
    Place& operator++(int) { // post-increment
        static Place copy(*this);
        if(dir == Direction::horizontal)
            coord.col++;
        else
            coord.row++;
        return copy;
    }
    Place& operator--(int) { // post-increment
        static Place copy(*this);
        if(dir == Direction::horizontal)
            coord.col--;
        else
            coord.row--;
        return copy;
    }
    Place operator+(const Place& arg) const {
        return Place(coord.row+arg.coord.row, coord.col+arg.coord.col, dir);
    }
    Place operator-(const Place& arg) const {
        return Place(coord.row-arg.coord.row, coord.col-arg.coord.col, dir);
    }
    Place operator+(const Coord& v) const {
        return Place(coord+v, dir);
    }
    Place operator-(const Coord& v) const {
        return Place(coord-v, dir);
    }
    Place& operator+=(const Coord& c) {
        coord+=c;
        return *this;
    }
    Place& operator-=(const Coord& c) {
        coord-=c;
        return *this;
    }
    int distL1(Place& a, Place& b) {
        Place p(a-b);
        return abs(p.coord.row)+abs(p.coord.col); }

    bool operator==(const Place& other) const {
        return std::tie(this->coord.row, this->coord.col, this->dir) ==
                std::tie(other.coord.row, other.coord.col, other.dir);
    }
    bool operator!=(const Place& other) const {
        return ! this->operator==(other);
    }
    bool operator<(const Place& other) const {
        return std::tie(this->coord.row, this->coord.col, this->dir) < std::tie(other.coord.row, other.coord.col, other.dir);
    }
    friend bool operator<( Place& l, const Place& r) {
        return l.operator<(r);
    }
    int getrow() const { return coord.row; }
    int getcol() const { return coord.col; }
    bool is_row() const { return dir == Direction::horizontal; }
    bool is_col() const { return dir == Direction::vertical; }
    Direction getdir() const { return dir; }
};

std::ostream& operator<<(std::ostream& out, const Place& place);

#endif /* Place_h */
