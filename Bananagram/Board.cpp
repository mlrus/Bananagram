//
//  Board.cpp
//  bananagram
//
//  Created by Michah Lerner on 2/9/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#include <numeric>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "Board.h"
#include "Place.h"
#include "simple_utils.h"

using namespace std;


bool Board::newsolve(deque<const Coord> &expanders) {
    if(debug) cout << "DEPTH " << depth <<" newsolve has " << expanders.size() << " expanders: " << expanders << endl;
    if(check_if_done()) {
        return true;
    }
    depth++;
    vector<const CharAtPos> uses;
    while(!expanders.empty()) {
        Coord coord(expanders.front());
        expanders.pop_front();
#if 0
        // Use same word order at each layer.
        for(auto word : dictionary.words) {
#else
        // Alternate word order between layers.
        Dictionary::worditerator words(dictionary.words,depth%2==0);
        for(auto word = words.begin(); words.has_next(); word = words.next()) {
#endif
            deque<const Place> new_expandables = word_starts(word,coord);
            if(!new_expandables.empty()) {
                for(const Place expand_at : new_expandables) {
                    if(insert_word(word, expand_at, uses)) {
                        if(!check_if_done()) {
                            deque<const Coord> expand_again(expanders.begin(),expanders.end()); // tail of expanders
                            for(const CharAtPos cap : uses) // letters just added
                                expand_again.push_front(cap.coord);
                            if(!newsolve(expand_again)) { // (early exit hook)
                                depth--;
                                return false;
                            }
                        }
                        revert(uses);
                        if(numunique >= max_results) {
                            depth--;
                            return false;
                        }
                    }
                }
            }
        }
    }
    depth--;
    return true;
}

bool Board::check_if_done() {
    if(num_unplayed()==0) {
        numresults++;
        ostringstream ostr;
        print_std(ostr);
        string st(ostr.str());
        if(boards_seen.count(st)==0) {
            numunique++;
            cout << "total=" << numresults
            << " unique=" << numunique
            << " (" << trunc(0.5 + 100.0 * numunique / numresults) << "%)\n";
            cout << st << "\n";
            boards_seen.insert(st);
#if 0
            if(numunique%500==0) {
                cout << "======================\n";
                for(auto p : board_counts)
                    cout << p.second << p.first;
                cout << "======================\n";
            }
#endif
        }
        ostr.str("");
        print_machine(ostr);
        board_counts[ostr.str()]++;
        
        return true;
    }
    return false;
}

void Board::revert(vector<const CharAtPos>& uses) {
    if(uses.size()==0) return;
    for(CharAtPos used : uses) {
        char ch = used.dir;
        Coord c = used.coord;
        ushort x = c.row;
        ushort y = c.col;
        board[x][y] = POS_UNUSED;
        unplayed[ch-'A']++;
    }
    std::vector<const CharAtPos>().swap(uses);
    uses.clear();
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
        throw runtime_error("Invalid position " + to_string(place.coord.row) + "," + to_string(place.coord.col) +  " (is dimension to small?)");
    return (unplayed[ch-'A'] > 0 && tile_at(place) == POS_UNUSED && check_artifacts(ch, place));
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
            unplayed[ch-'A']--;
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


const string Board::show_unplayed() const {
    std::stringstream result;
    for(int i = 0; i < unplayed.size(); i++) {
        if(unplayed[i] > 0)
            result <<  char('A'+i) << "*" << unplayed[i] << " ";
    }
    return result.str();
}

int Board::num_unplayed() const { return std::accumulate(unplayed.cbegin(),unplayed.cend(),0); }

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

bool Board::peel(int n) {
    std::random_shuffle(tiles.begin(), tiles.end());
    for(int i = 0; i < n && tiles.size() > 0 ; i++) {
        unplayed[tiles.back() - 'A']++;
        tiles.pop_back();
    }
    return true;
}

bool Board::peel(vector<char> letters) {
    vector<char>used;
    for(char ch : letters) {
        auto tile_at=std::find(tiles.begin(),tiles.end(),ch);
        if(tile_at != tiles.end()) {
            tiles.erase(tile_at);
            unplayed[ch - 'A']++;
            used.push_back(ch);
        } else {
            for(char ch : used) {
                tiles.push_back(ch);
                unplayed[ch - 'A']--;
            }
            return false;
        }
    }
    
    return true;
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
        if(num_unplayed()>0)
            out << "\n" << num_unplayed() << " unplayed: " <<  show_unplayed() << endl;
    }
    out << "\n\n";
}
