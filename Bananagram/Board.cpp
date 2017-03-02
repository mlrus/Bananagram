//
//  Board.cpp
//  bananagram
//
//  Created by Michah Lerner on 2/9/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <deque>
#include <functional>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Board.h"


using namespace std;

static const char POS_UNUSED = '_';

bool Board::newsolve(deque<const Coord> &expanders) {
    depth++;
    if(debug) {
        cout << "DEPTH " << depth <<" newsolve has " << expanders.size() << " expanders.\n";
        print_debug(cout);
        cout << expanders;
    }
    
    while(!expanders.empty()) {
        Coord coord(expanders.front());
        expanders.pop_front();
        // for(auto word : dictionary.words) {
        Dictionary::worditerator words(dictionary.words,depth%2==0);
        for(auto word = words.begin(); words.has_next() && numunique < max_results; word = words.next()) {
            deque<const Place> word_expand = word_starts(word,coord);
            if(debug) {
                cout << "\n+consider " << word;
                cout << " " << word_expand.size() << " expanders\n";
                print_debug(cout);
            }
            for(const Place expand_at : word_expand) {
                vector<const CharAtPos> uses;
                if(insert_word(word, expand_at, uses)) {
                    if(!is_solution()) {
                        deque<const Coord> expand_again(expanders.begin(),expanders.end());
                        for(const CharAtPos cap : uses)
                            expand_again.push_front(cap.coord);
                        newsolve(expand_again);
                    }
                    revert(uses);
                }
            }
        }
    }
    depth--;
    return true;
}

bool Board::is_new_solution() {
    ostringstream ostr;
    print_machine(ostr);
    string st(ostr.str());
    if(boards_seen.count(st)==0) {
        boards_seen.insert(st);
        numunique++;
        return true;
    }
    return false;
}

bool Board::is_solution() {
    if(num_unplaced()!=0)
        return false;
    numresults++;
    if(is_new_solution())
    if(is_new_solution()) {
        print_std(cout);
    }
    ostringstream ostr;
    print_machine(ostr);
    board_counts[ostr.str()]++;
    
    return true;
}

void Board::revert(vector<const CharAtPos>& uses) {
    while(!uses.empty()) {
        char ch = uses.back().dir;
        Coord c = uses.back().coord;
        ushort x = c.row;
        ushort y = c.col;
        board[x][y] = POS_UNUSED;
        unplaced[ch-'A']++;
        uses.pop_back();
    }
}


void Board::collect(Place p, const Coord& step, const string& word, char ch, deque<const Place>& result) {
    if(step.row<0||step.col<0) {
        string::const_iterator it = word.cbegin();
        p+=step;
        it++;
        // Assumes the board dimension (-D XX) is large enough.
        for( ; tile_at(p) == POS_UNUSED && it != word.cend(); p+=step) {
            if(*it++ == ch) {
                result.push_back(p);
            }
        }
    }
    else {
        string::const_reverse_iterator it = word.crbegin();
        Place d(p+step);
        it++;
        for( ; tile_at(d) == POS_UNUSED && it != word.crend(); d+=step) {
            if(*it++ == ch) {
                result.push_back(p);
            }
        }
    }
}

deque<const Place> Board::word_starts(const string& word, const Coord& coord) {
    deque<const Place> result;
    const char ch = board[coord.row][coord.col];
    collect(Place(coord,horz), Place::left, word, ch, result);
    collect(Place(coord,horz), Place::right, word, ch, result);
    collect(Place(coord,vert), Place::up, word, ch, result);
    collect(Place(coord,vert), Place::down, word, ch, result);
    return result;
}

bool Board::is_char_viable(const char ch, const Place& place) const {
    if(debug && (place.coord.row==0 || place.coord.row==dim-1 || place.coord.col==0 || place.coord.col==dim-1))
        throw runtime_error("Invalid position "
                            + to_string(place.coord.row) + ","
                            + to_string(place.coord.col) + " (is dimension to small?)");
    return (unplaced[ch-'A'] > 0 && tile_at(place) == POS_UNUSED && check_artifacts(ch, place));
}

bool Board::is_word_viable(const string& word, const Place& place) const {
    if(place.is_row()) {
        if(tile_at(place+Place::left) != POS_UNUSED ||
           place.coord.col + word.length() >= dim ||
           tile_at(place+Coord((unsigned long)(0),word.length())) != POS_UNUSED) {
            return false;
        }
    } else {
        if(tile_at(place+Place::up)!= POS_UNUSED ||
           place.coord.row + word.length() >= dim ||
           tile_at(place+Coord(word.length(),(unsigned long)(0))) != POS_UNUSED) {
            return false;
        }
    }
    return true;
}

bool Board::is_word_at(const string& word, const Place& place) {
    Place here(place);
    for(char ch : word) {
        if(tile_at(here++) != ch) {
            return false;
        }
    }
    return true;
}

/*
 Intersect the word to the board, saving changes to "uses".
 */
bool Board::try_insert(const string& word, const Place& place, vector<const CharAtPos>& uses) {
    return insert_word(word,place,uses,true);
}

// Check the word which is perpendicular to place
bool
Board::check_artifacts(char ch, const Place& place) const {
    Place::Direction perpendicular =
    place.dir==Place::Direction::horizontal?
    Place::Direction::vertical:
    Place::Direction::horizontal;
    
    Place loc(place.coord,perpendicular);
    Place before = loc;
    Place after  = loc;
    string word_to_check;
    word_to_check.push_back(ch);
    char side_ch;
    while( (side_ch = tile_at(--before)) != POS_UNUSED )
        word_to_check.push_back(side_ch);
    reverse(word_to_check.begin(),word_to_check.end());
    while( (side_ch = tile_at(++after)) != POS_UNUSED )
        word_to_check.push_back(side_ch);
    
    if(word_to_check.length() > 1) {
        return dictionary.has(word_to_check);
    }
    return true;
}


/* Initialize the board with the given word */
bool Board::insert_word(const string& word, const Place& place, vector<const CharAtPos>& uses, bool must_share) {
    if(!is_word_viable(word,place)) //assuming this makes it faster, but maybe not
        return false;
    Place here(place);
    int num_shared = 0;
    for(char ch : word) {
        if(tile_at(here) == ch) {
            num_shared++;
            here++;
            continue;
        }
        if(is_char_viable(ch,here)) {
            uses.push_back(CharAtPos(ch,here.coord));
            board[here.coord.row][here.coord.col] = ch;
            unplaced[ch-'A']--;
            here++;
            continue;
        }
        revert(uses);
        return false;
    }
    
    if(!must_share || num_shared > 0 || uses.size() == 0)
        return true;
    revert(uses);
    return false;
}


const string Board::show_unplaced() const {
    std::stringstream result;
    for(int i = 0; i < unplaced.size(); i++) {
        if(unplaced[i] > 0)
            result <<  char('A'+i) << "*" << unplaced[i] << " ";
    }
    return result.str();
}

int Board::num_unplaced() const { return std::accumulate(unplaced.cbegin(),unplaced.cend(),0); }

bool Board::empty_row(int i) const {
    for(int j=0; j < dim; j++)
        if(board[i][j] != POS_UNUSED)
            return false;
    return true;
}

bool Board::empty_col(int j) const {
    for(int i=0; i < dim; i++)
        if(board[i][j] != POS_UNUSED)
            return false;
    return true;
}

int Board::first_nonempty_row() const {
    for(int i=0;i<dim;i++)
        if(!empty_row(i)) return i;
    return -1;
}

int Board::last_nonempty_row() const {
    for(int i=dim-1; i>=0; i--)
        if(!empty_row(i)) return i;
    return -1;
}


int Board::first_nonempty_col() const {
    for(int i=0;i<dim;i++)
        if(!empty_col(i)) return i;
    return -1;
}

int Board::last_nonempty_col() const {
    for(int i=dim-1; i>=0; i--)
        if(!empty_col(i)) return i;
    return -1;
}

bool Board::empty() const {
    return first_nonempty_row()<0 || first_nonempty_col()<0;
}

void Board::enable_playable_words() {
    dictionary.make_playable(dictionary.all_words,unplaced);
}

void Board::unpeel(char ch) {
    tiles.push_back(ch);
    counts_unseen[ch - 'A']++;
    unplaced[ch - 'A']--;
    if(unplaced[ch - 'A'] < 0)
        throw runtime_error("Invalid unpeel of unplayed tile.");
}

vector<char> Board::peel(char ch) {
    vector<char> result;
    //cout << "peel '" << ch << "' from " << tiles.size() << " tiles, ";
    vector<char>::const_iterator where=std::find(tiles.begin(),tiles.end(),ch);
    if(where!=tiles.end()) {
        tiles.erase(where);
        result.push_back(ch);
        counts_unseen[ch-'A']--;
        unplaced[ch-'A']++;
    }
    return result;
}

vector<char> Board::peel(int n) {
    vector<char> result;
    //std::random_shuffle(tiles.begin(), tiles.end());
    for(int i = 0; i < n && tiles.size() > 0 ; i++) {
        result.push_back(tiles.back());
        tiles.pop_back();
    }
    for(char ch : result) {
        counts_unseen[ch-'A']--;
        unplaced[ch-'A']++;
    }
    return result;
}

vector<char> Board::peel(vector<char> letters) {
    vector<char>result;
    for(char ch : letters) {
        vector<char>::const_iterator where=std::find(tiles.begin(),tiles.end(),ch);
        if(where!=tiles.cend()) {
            result.push_back(ch);
            tiles.erase(where);
        }
    }
    for(char ch : result) {
        counts_unseen[ch-'A']--;
        unplaced[ch-'A']++;
    }
    return result;
}

void Board::print(ostream& out) const {
    if(0x04&output_options) print_machine(out);
    if(0x02&output_options) print_debug(out);
    if(0x01&output_options) print_std(out);
}

void Board::print_std(ostream& out) const {
    if(!empty()) {
        int from_row = first_nonempty_row();
        int to_row = last_nonempty_row();
        int from_col = first_nonempty_col();
        int to_col = last_nonempty_col();
        for(int i = from_row; i <= to_row; i++) {
            for(int j = from_col; j <= to_col; j++) {
                out << this->board[i][j];
            }
            out << "\n";
        }
    }
    out << "\n";
}

void Board::print_machine(ostream& out) const {
    if(!empty()) {
        int from_row = first_nonempty_row();
        int to_row = last_nonempty_row();
        int from_col = first_nonempty_col();
        int to_col = last_nonempty_col();
        out << "### ";
        for(int i = from_row; i <= to_row; i++) {
            for(int j = from_col; j <= to_col; j++) {
                out << board[i][j];
            }
            out << ":";
        }
        out << "\n";
    }
}

void Board::print_debug(ostream& out) const {
    if(!empty()) {
        int from_row = first_nonempty_row();
        int to_row = last_nonempty_row();
        int from_col = first_nonempty_col();
        int to_col = last_nonempty_col();
        out << "     | ";
        for(int j = from_col; j <= to_col; j++)
            out << std::setw(1) << (j/10)%10;
        out << "\n     | ";
        for(int j = from_col; j <= to_col; j++)
            out << std::setw(1) << j % 10;
        for(int i = from_row; i <= to_row; i++) {
            out << "\n" << std::setw(4) << i << " | ";
            for(int j = from_col; j <= to_col; j++) {
                out << Board::board[i][j];
            }
        }
        if(num_unplaced()>0)
            out << "\n" << num_unplaced() << " unplaced: " <<  show_unplaced() << endl;
    }
    out << "\n\n";
}
