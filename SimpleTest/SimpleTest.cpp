//
//  SimpleTest.cpp
//  bananagram
//
//  Created by Michah Lerner on 2/14/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <ostream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <utility>
#include <vector>

#include "defs.h"
#include "simple_utils.h"
#include "Dictionary.h"
#include "Board.h"
#include "Place.h"

TEST(DictionaryTest,CountsCorrectly) {
    Dictionary d;
    d.add_words({"one","two","three"});
    EXPECT_EQ(d.words.size(),3);
}

TEST(DictionaryTest,NoDuplicates) {
    Dictionary d;
    d.add_words({"one","two","three"});
    d.add_words({"one","two","three"});
    EXPECT_EQ(d.words.size(),3);
}

TEST(DictionaryTest,Uppercase) {
    Dictionary d;
    vector<string>wlist({"one","two","three","oNe","Two","thrEE"});
    d.add_words(wlist);
    EXPECT_EQ(d.words.size(),3);
    EXPECT_EQ(std::count(std::begin(d.words),std::end(d.words),"TWO"),1);
    EXPECT_TRUE(d.has("ONE"));
    EXPECT_FALSE(d.has("one"));
}

TEST(DictionaryTest,Fail) {
    Dictionary d;
    d.add_words({"one","two","three"});
    EXPECT_FALSE(d.has("one"));
}


TEST(Foo,Bar) {
    int count = 0;
    EXPECT_EQ(count,0);
}

TEST(utils,ostream_scalars) {
    std::ostringstream oss;
    const Place c_place(1,2,'r');
    const Coord c_coord(1,2);
    Coord coord(1,2);
    const char_at_pos cap('X',pair<int,int>(1,2));
    oss << c_place;
    EXPECT_EQ(oss.str(),"1:2:horz ");
    oss.str("");
    oss << c_coord;
    EXPECT_EQ(oss.str(),"(1,2) ");
    oss.str("");
    oss << coord;
    EXPECT_EQ(oss.str(),"(1,2) ");
    oss.str("");
    oss << cap;
    EXPECT_EQ(oss.str(),"X@(1,2) ");
    oss.str("");
}

TEST(utils,ostream_containers) {
    std::ostringstream oss;
    const Place c_place(1,2,'r');
    const Coord c_coord(1,2);
    Coord coord(1,2);
    const char_at_pos cap('X',pair<int,int>(1,2));
    
    vector<const Place> v1({c_place,c_place});
    vector<const Coord> v2({c_coord,c_coord});
    vector<Coord> v3({coord,coord});
    vector<const char_at_pos> v4({cap,cap});
    vector<char> v5({'a','b','c'});
    vector<vector<char>> v6({v5,v5});
    deque<const Place> v7({c_place,c_place});
    oss << v1;
    EXPECT_EQ(oss.str(),"1:2:horz 1:2:horz ");
    oss.str("");
    oss << v2;
    EXPECT_EQ(oss.str(),"(1,2) (1,2) ");
    oss.str("");
    oss << v3;
    EXPECT_EQ(oss.str(),"(1,2) (1,2) ");
    oss.str("");
    oss << v4;
    EXPECT_EQ(oss.str(),"X@(1,2)  X@(1,2)  ");
    oss.str("");
    oss << v5;
    EXPECT_EQ(oss.str(),"abc");
    oss.str("");
    oss << v6;
    EXPECT_EQ(oss.str(),"abc\nabc\n");
    oss.str("");
    oss << v7;
    EXPECT_EQ(oss.str(),"1:2:horz \n1:2:horz \n");

}

TEST(Place,initializers) {
    Place r5c6h(5,6,Place::Direction::horizontal);
    EXPECT_EQ(r5c6h.getrow(),5);
    EXPECT_EQ(r5c6h.getcol(),6);
    EXPECT_EQ(r5c6h.getdir(),Place::Direction::horizontal);
    
    Place r6c7v(6,7,'c');
    EXPECT_EQ(r6c7v.getrow(),6);
    EXPECT_EQ(r6c7v.getcol(),7);
    EXPECT_EQ(r6c7v.getdir(),Place::Direction::vertical);
}

TEST(Place,assignment) {
    Place r5c6h(5,6,Place::Direction::horizontal);
    Place r6c7v(6,7,'c');
    EXPECT_NE(r5c6h,r6c7v);
    Place newplace(r5c6h);
    EXPECT_EQ(r5c6h,newplace);
    newplace=r6c7v;
    EXPECT_EQ(r6c7v,newplace);
    EXPECT_NE(r5c6h,newplace);
}

TEST(Place,operators) {
    Place r5c6h(5,6,Place::Direction::horizontal);
    Place r6c7v(6,7,'c');
    r5c6h += pair<int,int>(1,0);
    EXPECT_EQ(r5c6h.getrow(),6);
    EXPECT_EQ(r5c6h.getcol(),6);
    EXPECT_EQ(r5c6h,Place(6,6,'r'));
    Place p = r6c7v;
    p = Place(0,0,'r');
    p += Place::right;
    EXPECT_EQ(p.getrow(),0);
    EXPECT_EQ(p.getcol(),1);
}

TEST(place,errs) {
    EXPECT_ANY_THROW({Place r6c7v(6,7,'z');} );
    ASSERT_THROW({Place r6c7v(6,7,'z');},out_of_range);
}

TEST(board,accept) {
    Dictionary dictionary;
    dictionary.add_words({"THE","CAT"});
    std::vector<char> tiles(initialize_tiles());
    Board board(dictionary, tiles);
    vector<char>letters(tochar("THECAT"));
    if(!board.peel(letters)) {
        cout << "Cannot assign the letters requested.\n";
        ASSERT_EQ(true,false);
    }
    Coord initial_pos(board.dim/2,board.dim/2);
    vector<const char_at_pos> uses;
    board.insert_word("CAT",Place(5,5,Place::horizontal),uses);
    
    cout << board;
}


Board define_board(string letterstring="BASEDHABITJOTTEDIALFFOROO") {
    int dim = 16;
    Dictionary dictionary;
    dictionary.add_words({"IF","AFT","ALOES","TEAR", "HIT","DO","FOE","BARD","DO","BASH","JOT",});
    std::vector<char> tiles(initialize_tiles());
    Board board(dictionary, tiles, dim);
    vector<char>letters(tochar(letterstring));
    if(!board.peel(letters)) {
        cout << "Cannot assign the letters requested.\n";
    }
    return board;
}


/*
 A*3 B*2 D*2 E*2 F*2 H*1 I*2 J*1 L*1 O*4 R*1 S*1 T*3
 
 SOLUTION
 | 01234567890123456
 0 | _________________
 1 | _________I_T_____
 2 | _________FOE_____
 3 | ___________A_____
 4 | _________BARD____
 5 | __________L_O____
 6 | _________DO______
 7 | __________E______
 8 | ________BASH_____
 9 | _________F_I_____
 10 | _______JOT_T_____
 11 | _________________
 12 | _________________
 13 | _________________
 14 | _________________
 15 | _________________
 16 | _________________
 
 
 or
 
 | 01234567890123456
 0 | _________________
 1 | _________________
 2 | _________________
 3 | _________________
 4 | _________________
 5 | _________________
 6 | ____________J____
 7 | __________B_O____
 8 | ___IF_B_D_AFT____
 9 | ____O_ALOES______
 10 | ___TEAR___HIT____
 11 | ______DO_________
 12 | _________________
 13 | _________________
 14 | _________________
 15 | _________________
 16 | _________________
 
 */

TEST(board,collect_a) {
    string letterstring("BASEDHABITJOTTEDIALFFOROO");
    Board board(define_board(letterstring));
    vector<const char_at_pos> uses;
    vector<pair<string,Coord>> test_set({
        {"ALOES",Coord(8,12)},
        { "AFT",Coord(8,11)},
        { "DO",Coord(6,12)} } );
    board.debug = true;
    
    bool ins = board.insert_word("BASH",Place(8,10,Place::horizontal),uses);
    ASSERT_EQ(ins,true);
    cout << board << endl;
   
    for(auto test : test_set) {
        string word(test.first);
        Coord coord(test.second);
        if(board.debug) cout << "Collect " << word << " from " << coord << endl;
        deque<const Place> res = board.word_starts(word,coord);
        if(board.debug)
            cout << "collected: " << res << endl;
        
        bool any = false;
        for(auto p : res) {
            bool ins = board.insert_word(word,p,uses);
            any |= ins;
            if(ins) {
                cout << "INSERTED " << word << " at " << p << endl;
                cout << board << endl;
            }
            else
                if(board.debug) cout << "skipping " << word << " at " << p << endl;
        }
        ASSERT_EQ(any,true);
    }
    board.revert(uses);
}

TEST(board,collect_b) {
    string letterstring("BASEDHABITJOTTEDIALFFOROO");
    Board board(define_board(letterstring));
    board.debug=false;
    vector<const char_at_pos> uses;
    vector<pair<string,Coord>> test_set({
        { "ALOES",Coord(6,12)},
        { "BASH", Coord(8,12)},
        { "AFT",Coord(8,11)} });

    bool ins = board.insert_word("DO",Place(6,11,Place::horizontal),uses);
    ASSERT_EQ(ins,true);
    cout << board << endl;
    
    for(auto test : test_set) {
        string word(test.first);
        Coord coord(test.second);
        if(board.debug) cout << "Collect " << word << " from " << coord << endl;
        deque<const Place> res = board.word_starts(word,coord);
        for(auto p : res) cout << "collect: " << p << endl;
        bool any = false;
        for(auto p : res) {
            bool ins = board.insert_word(word,p,uses);
            any |= ins;
            if(ins) {
                cout << "INSERTED " << word << " at " << p << endl;
                cout << board << endl;
            }
            else
                if(board.debug) cout << "skipping " << word << " at " << p << endl;
        }
        ASSERT_EQ(any,true);
    }
    board.revert(uses);
  }