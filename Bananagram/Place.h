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

#include "defs.h"

using namespace std;

class Place {
public:
    enum Direction { horizontal, vertical };
public:
    int row, col;
    Direction direction;
public:
    static std::unordered_map<int, std::string> PlaceNames;
    static const std::pair<int,int> right, left, up, down;
    Place(int r, int c, Direction dir) : row(r), col(c), direction(dir) { }
    Place(const Coord& coord, Direction dir) : row(coord.first), col(coord.second), direction(dir) { }
    Place(Place &pl) : row(pl.row), col(pl.col), direction(pl.direction) { }
    Place(const Place &pl) : row(pl.row), col(pl.col), direction(pl.direction) { }
    Place(int r, int c, char d) : row(r),col(c), direction(d=='r'?Direction::horizontal:Direction::vertical) {
        if(d!='r'&&d!='c') throw std::out_of_range("direction_error");
    }
    virtual ~Place() { }
    
    //Place& operator=(Place arg) { std::swap(*this,arg); return *this; }
    Place& operator=(Place arg) {
        row = arg.row;
        col = arg.col;
        direction = arg.direction;
        return *this;
    }
    Place& operator++() { // pre-increment
        if(direction == Direction::horizontal)
            col++;
        else
            row++;
        return *this;
    }
    Place& operator--() { // pre-increment
        if(direction == Direction::horizontal)
            col--;
        else
            row--;
        return *this;
    }
    Place& operator++(int) { // post-increment
        static Place copy(*this);
        if(direction == Direction::horizontal)
            col++;
        else
            row++;
        return copy;
    }
    Place& operator--(int) { // post-increment
        static Place copy(*this);
        if(direction == Direction::horizontal)
            col--;
        else
            row--;
        return copy;
    }
    Place operator+(const Place& arg) const {
        return Place(row+arg.row, col+arg.col, direction);
    }
    Place operator-(const Place& arg) const {
        return Place(row-arg.row, col-arg.col, direction);
    }
    Place operator+(const std::pair<int,int>& v) const {
        return Place(row+v.first, col+v.second, direction);
    }
    Place& operator+=(const std::pair<int,int>& v) {
        row += v.first; col += v.second; return *this;
    }
    Place operator-(const std::pair<int,int>& v) const {
        return Place(row-v.first, col-v.second, direction);
    }
    Place& operator-=(const std::pair<int,int>& v) {
        row -= v.first; col -= v.second; return *this;
    }
    int distL1(Place& a, Place& b) { Place p = a-b; return abs(p.row)+abs(p.col); }

    bool operator==(const Place& other) const {
        return std::tie(this->row, this->col, this->direction) == std::tie(other.row, other.col, other.direction);
    }
    bool operator!=(const Place& other) const {
        return ! this->operator==(other);
    }
    bool operator<(const Place& other) const {
        return std::tie(this->row, this->col, this->direction) < std::tie(other.row, other.col, other.direction);
    }
    friend bool operator<( Place& l, const Place& r) {
        return l.operator<(r);
    }
    int getrow() const { return row; }
    int getcol() const { return col; }
    bool is_row() const { return direction == Direction::horizontal; }
    bool is_col() const { return direction == Direction::vertical; }
    Direction getdir() const { return direction; }
};

#endif /* Place_h */
