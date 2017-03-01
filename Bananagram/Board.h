//
//  Board.hpp
//  bananagram
//
//  Created by Michah Lerner on 2/9/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#ifndef Board_h
#define Board_h

#include <deque>
#include "simple_utils.h"
#include "defs.h"
#include "Dictionary.h"
#include "Place.h"


class Board {
    
public:
    int depth;
    vector<vector<char>> board;
    Dictionary dictionary;
    vector<char> tiles;
    vector<int> counts_unseen;
    vector<int> unplaced;
    unordered_set<string> boards_seen;
    unordered_map<string,int> board_counts;

    char tile_at(const Place& p) const { return board[p.getrow()][p.getcol()]; }
    bool is_char_viable(const char ch, const Place& place) const;
    bool is_word_viable(const string& word, const Place& place) const;
    bool is_word_at(const string& word, const Place& place);
    bool check_artifacts(const char ch, const Place& place) const;
    
    void collect(Place, const Coord&, const string&, char ch, deque<const Place>&);
    bool try_insert(const string& word, const Place& place,
                    vector<const CharAtPos>& uses);
    bool check_if_done();
    
public:
    static const bool as_row = true;
    static const bool as_col = !as_row;
    static const Place::Direction vert = Place::Direction::vertical;
    static const Place::Direction horz = Place::Direction::horizontal;
    const unsigned int dim;
    const unsigned int numtiles;
    unsigned int output_options;
    long long numresults;
    long long numunique;
    long long max_results;
    bool debug;

    void print(ostream& where) const;
    void print_std(ostream& where) const;
    void print_machine(ostream& where) const;
    void print_debug(ostream& where) const;
    
    Board(Dictionary& dictionary, const vector<char> & tiles, unsigned int d = 256, unsigned ntile = 12) :
    depth(0),
    board(vector<vector<char>>(d+1,vector<char>(d+1,'_'))),
    dictionary(dictionary),
    tiles(tiles),
    counts_unseen(vector<int>(26, 0)),
    unplaced(vector<int>(26, 0)),
    dim(d),
    numtiles(ntile),
    output_options(1),
    numresults(0),
    numunique(0),
    max_results(1000),
    debug(false) {
        for(char ch : tiles) counts_unseen[ch-'A']++;
    }

    
    bool newsolve(deque<const Coord>&);
    void unpeel(char ch);
    vector<char> peel(char ch);
    vector<char> peel(int n=12);
    vector<char> peel(vector<char> letters);
    void enable_playable_words();
    deque<const Place> word_starts(const string&word, const Coord& coord);
    bool insert_word(const string& word, const Place& place, vector<const CharAtPos>& uses, bool must_share=false);
    void revert(vector<const CharAtPos>& uses);
  
    int num_unplaced() const;
    const string show_unplaced() const;
    
    bool empty_row(int n) const;
    bool empty_col(int n) const;
    int first_nonempty_row() const;
    int last_nonempty_row() const;
    int first_nonempty_col() const;
    int last_nonempty_col() const;
    bool empty() const;
    friend ostream& operator<<(ostream&, const Board&);
    friend ostream& operator<<(ostream&, const Coord&);
};

#endif /* Board_h */
