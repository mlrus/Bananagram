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

#include "simple_utils.h"
#include "Dictionary.h"
#include "Board.h"
#include "Place.h"

TEST(Dummy,Test) {
    int count = 0;
    EXPECT_EQ(count,0);
}

TEST(DictionaryTest,CountsCorrectly) {
    Dictionary d;
    d.add_words({"one","two","three"});
    EXPECT_EQ(d.all_words.size(),3);
    EXPECT_EQ(d.words.size(),0);
    d.activate_word(d.all_words[0]);
    
}

TEST(DictionaryTest,NoDuplicates) {
    Dictionary d;
    d.add_words({"one","two","three"});
    d.add_words({"one","two","three"});
    EXPECT_EQ(d.all_words.size(),3);
    EXPECT_EQ(d.words.size(),0);
}

TEST(DictionaryTest,Uppercase_exists) {
    Dictionary d;
    vector<string>wlist({"one","two","three","oNe","Two","thrEE"});
    d.add_words(wlist);
    EXPECT_EQ(d.all_words.size(),3);
    EXPECT_EQ(d.words.size(),0);
    EXPECT_EQ(std::count(std::begin(d.all_words),std::end(d.all_words),"TWO"),1);
    EXPECT_TRUE(d.exists("ONE"));
    EXPECT_FALSE(d.exists("one"));
}

TEST(DictionaryTest,Uppercase_has) {
    Dictionary d;
    vector<string>wlist({"one","two","three","oNe","Two","thrEE"});
    d.add_words(wlist);
    EXPECT_EQ(d.all_words.size(),3);
    EXPECT_EQ(d.words.size(),0);
    EXPECT_EQ(std::count(std::begin(d.all_words),std::end(d.all_words),"TWO"),1);
    EXPECT_FALSE(d.has("one"));
    EXPECT_FALSE(d.has("ONE"));
    d.activate_word("one");
    EXPECT_FALSE(d.has("one"));
    EXPECT_TRUE(d.has("ONE"));
}

TEST(DictionaryTest,Fail) {
    Dictionary d;
    d.add_words({"one","two","three"});
    EXPECT_FALSE(d.has("one"));
}

TEST(DictionaryTest,plausibility) {
    Dictionary d;
    string word("APPLE");
    vector<int> counts(d.letter_counts(word));
    EXPECT_TRUE(d.plausible(word,counts));
    counts[0]--;
    EXPECT_FALSE(d.plausible(word,counts));
    for(int& v : counts) v = 0;
    string w("XXXXXXXXXXXXXXXXXXXX");
    counts['X'-'A'] = int(w.length());
    EXPECT_TRUE(d.plausible(w,counts));
    w.push_back('X');
    EXPECT_FALSE(d.plausible(w,counts));
}

TEST(DictionaryTest,verify_plausible) {
    Dictionary d;
    vector<char> tiles(initialize_tiles());
    vector<int> max_peel(26,0);
    for(char ch : tiles) max_peel[ch - 'A']++;
    
    vector<string>ok_words({{"zebraone","zerbratwo","zebrathree","four","five","six"}});
    vector<string>invalidWords({{"zatsatzetze","juicyjamjot","zatzatza"}});
    vector<string>all_words(ok_words.begin(), ok_words.end());
    copy(invalidWords.begin(),invalidWords.end(),back_inserter(all_words));
    d.add_words(all_words);
    EXPECT_EQ(max_peel['Z'-'A'],2);
    EXPECT_EQ(max_peel['J'-'A'],2);
    
    for(const string w : ok_words)
        EXPECT_TRUE(d.plausible(to_upper(w), max_peel));
    for(const string w : invalidWords)
        EXPECT_FALSE(d.plausible(to_upper(w), max_peel));
}

TEST(DictionaryTest,verify_playable) {
    Dictionary d;
    vector<char> tiles(initialize_tiles());
    vector<int> max_peel(26,0);
    for(char ch : tiles) max_peel[ch - 'A']++;
    
    vector<string>ok_words({{"zebraone","zerbratwo","zebrathree","four","five","six"}});
    vector<string>all_words(ok_words.begin(), ok_words.end());
    d.add_words(all_words);

    for(const string w: all_words) {
        ASSERT_TRUE(d.exists(to_upper(w)));
        ASSERT_FALSE(d.has(to_upper(w)));
    }
    
    vector<string>first_3_words(ok_words.begin(),next(ok_words.begin(),2));
    vector<string>last_3_words(next(ok_words.begin(),3), ok_words.end());

    ASSERT_TRUE(d.make_playable(first_3_words,max_peel));
    for(string w : first_3_words) {
        ASSERT_TRUE(d.has(to_upper(w)));
    }
    for(string w : last_3_words) {
        ASSERT_FALSE(d.has(to_upper(w)));
    }
}

TEST(utils,ostream_scalars) {
    std::ostringstream oss;
    const Place c_place(1,2,'r');
    const Coord c_coord(1,2);
    Coord coord(1,2);
    const CharAtPos cap('X',Coord(1,2));
    oss << c_place;
    EXPECT_EQ(oss.str(),"(1,2):horz");
    oss.str("");
    oss << c_coord;
    EXPECT_EQ(oss.str(),"(1,2)");
    oss.str("");
    oss << coord;
    EXPECT_EQ(oss.str(),"(1,2)");
    oss.str("");
    oss << cap;
    EXPECT_EQ(oss.str(),"X@(1,2)");
    oss.str("");
}

TEST(utils,ostream_containers) {
    std::ostringstream oss;
    const Place c_place(1,2,'r');
    const Coord c_coord(1,2);
    Coord coord(1,2);
    const CharAtPos cap('X',Coord(1,2));
    
    vector<const Place> v1({c_place,c_place});
    vector<const Coord> v2({c_coord,c_coord});
    vector<Coord> v3({coord,coord});
    vector<const CharAtPos> v4({cap,cap});
    vector<char> v5({'a','b','c'});
    vector<vector<char>> v6({v5,v5});
    deque<const Place> v7({c_place,c_place});
    oss << v1;
    EXPECT_EQ(oss.str(),"(1,2):horz; (1,2):horz; ");
    oss.str("");
    oss << v2;
    EXPECT_EQ(oss.str(),"(1,2); (1,2); ");
    oss.str("");
    oss << v3;
    EXPECT_EQ(oss.str(),"(1,2); (1,2); ");
    oss.str("");
    oss << v4;
    EXPECT_EQ(oss.str(),"X@(1,2); X@(1,2); ");
    oss.str("");
    oss << v5;
    EXPECT_EQ(oss.str(),"abc");
    oss.str("");
    oss << v6;
    EXPECT_EQ(oss.str(),"abc\nabc\n");
    oss.str("");
    oss << v7;
    EXPECT_EQ(oss.str(),"(1,2):horz; (1,2):horz; ");

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
    r5c6h += Coord(1,0);
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
    vector<char>peeled(board.peel(letters));
    if(peeled.size()!=letters.size()) {
        cout << "Cannot assign the letters requested.\n";
        ASSERT_EQ(true,false);
    }
    Coord initial_pos(int(board.dim/2),int(board.dim/2));
    vector<const CharAtPos> uses;
    board.insert_word("CAT",Place(5,5,Place::horizontal),uses);
    
    cout << board;
}

TEST(dictionary,iterator_fw) {
    Dictionary dictionary(true);
    vector<string>words({{"one","two","three"}});
    dictionary.add_words(words);
    dictionary.activate_words(dictionary.all_words);
    Dictionary::worditerator iter(dictionary.all_words);
    ASSERT_EQ("ONE",iter.next());
    ASSERT_EQ("TWO",iter.next());
    ASSERT_EQ("ONE",iter.begin());
    ASSERT_EQ("TWO",iter.next());
    ASSERT_EQ("THREE",iter.next());
    ASSERT_FALSE(iter.has_next());
}

TEST(dictionary,iterator_fw_literals) {
    Dictionary dictionary(true);
    dictionary.add_words({{"one","two","three"}});
    dictionary.activate_words(dictionary.all_words);
    Dictionary::worditerator iter(dictionary.all_words);
    ASSERT_EQ("ONE",iter.next());
    ASSERT_EQ("TWO",iter.next());
    ASSERT_EQ("ONE",iter.begin());
    ASSERT_EQ("TWO",iter.next());
    ASSERT_EQ("THREE",iter.next());
    ASSERT_FALSE(iter.has_next());
}

TEST(dictionary,iterator_bw) {
    Dictionary dictionary(true);
    dictionary.add_words({{"one","two","three"}});
    Dictionary::worditerator iter(dictionary.all_words,false);
    ASSERT_EQ("THREE",iter.next());
    ASSERT_EQ("TWO",iter.next());
    ASSERT_EQ("THREE",iter.begin());
    ASSERT_EQ("TWO",iter.next());
    ASSERT_EQ("ONE",iter.next());
    ASSERT_FALSE(iter.has_next());
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

Board define_board(string letterstring="BASEDHABITJOTTEDIALFFOROO",
                   vector<string> vocab={"IF","AFT","ALOES","TEAR", "HIT","DO","FOE","BARD","DO","BASH","JOT"},
                   int dim=16) {
    cout << "=================\n";
    Dictionary dictionary(false);
    dictionary.add_words(vocab);
    std::vector<char> tiles(initialize_tiles());
    Board board(dictionary, tiles, dim);
    board.debug = false;
    cout << "define_board::board.dictionary.all_words size is " <<
    dictionary.all_words.size() << " = " << board.dictionary.all_words.size() << endl;
    if(board.peel(tochar(letterstring)).size()!=letterstring.size()) {
        cout << "Cannot assign the letters requested.\n";
    }
    return board;
}

bool run_board_test(Board& board, vector<pair<string,Coord>>& test_set, bool trace=false) {
    vector<const CharAtPos>uses;
    bool any = false;
    int n = 0;
    for(auto test : test_set) {
        string word(test.first);
        Coord coord(test.second);
        if(trace) cout << "STEP " << setw(2) << ++n << ") word_starts(" << word << "," << coord << ")\n";
        deque<const Place> res = board.word_starts(word,coord);
        if(trace) cout << "collected:\n" << res;
        int m = 0;
        for(auto p : res) {
            if(trace) cout << "  " << n << "." << ++m << " attempt insert("<<word<<","<<p<<") ";
            bool ins = board.insert_word(word,p,uses);
            any |= ins;
            if(ins)
                cout << "    [OK]  (|uses|=" << uses.size() << ")\n";
            else
                cout << "    [NIX] (|uses|=" << uses.size() << ")\n";
            
            if(!uses.empty()) cout << uses << endl;
            cout << board << endl;
        }
    }
    return any;
}

TEST(board,collect_a) {
    cout << "collect_a\n";
    Board board(define_board());
    cout <<"_\n";
    vector<const CharAtPos> uses;
    
    vector<pair<string,Coord>> test_set({
        {"ALOES",Coord(8,12)},
        { "AFT",Coord(8,11)},
        { "DO",Coord(6,12)} } );

    ASSERT_TRUE(board.insert_word("BASH",Place(8,10,Place::horizontal),uses));
    ASSERT_TRUE(run_board_test(board, test_set));
}

TEST(board,collect_b) {
    Board board(define_board());
    vector<const CharAtPos> uses;
    vector<pair<string,Coord>> test_set({
        { "ALOES",Coord(6,12)},
        { "BASH", Coord(8,12)},
        { "AFT",Coord(8,11)} });

    ASSERT_TRUE(board.insert_word("DO",Place(6,11,Place::horizontal),uses));
    cout << board;
    ASSERT_TRUE(run_board_test(board, test_set, true));
}


TEST(board,peeling) {
    string initial_letters("IFHT");
    /* Only words 'IF' and 'HIT' are formed from te given letters. */
    Board board(define_board(initial_letters));
    for(auto w : board.dictionary.all_words) {
        EXPECT_TRUE(board.dictionary.exists(w));
        EXPECT_FALSE(board.dictionary.has(w));
    }
    board.enable_playable_words();
    for(auto w : board.dictionary.all_words)
        cout <<w<<" exists=" << boolalpha << board.dictionary.exists(w) << " has=" << boolalpha<< board.dictionary.has(w) << "\n";
    ASSERT_TRUE(board.dictionary.has("IF"));       // active
    ASSERT_TRUE(board.dictionary.has("HIT"));      // active
    ASSERT_TRUE(board.dictionary.exists("AFT"));   // exists (not active)
    ASSERT_FALSE(board.dictionary.has("AFT"));     // not active
    ASSERT_FALSE(board.dictionary.has("TAF"));     // not active ...
    ASSERT_FALSE(board.dictionary.exists("TAF"));  // ... or existing
    ASSERT_TRUE(board.peel('A').size()==1);        // This activates AFT
    board.enable_playable_words();
    ASSERT_TRUE(board.dictionary.has("IF"));
    ASSERT_TRUE(board.dictionary.has("HIT"));
    ASSERT_TRUE(board.dictionary.has("IF"));
    ASSERT_TRUE(board.dictionary.has("AFT"));
    ASSERT_FALSE(board.dictionary.has("TAF"));
}


TEST(board,fitting_001) {
    {
    Board board(define_board("TACAT",{"TAC","CAT"}));
    vector<const CharAtPos> uses;
    vector<pair<string,Coord>> test_set({
        {"CAT",Coord(8,10)},
        {"TAC",Coord(4,8)},
    });
    ASSERT_TRUE(board.insert_word("TAC",Place(8,8,Place::horizontal),uses));
    cout << board << endl;
    ASSERT_TRUE(run_board_test(board, test_set, true));
    cout << board << endl;
    }
    {
    Board board(define_board("TACAT",{"CAT","TAC"}));
    vector<const CharAtPos> uses;
    vector<pair<string,Coord>> test_set({
        {"CAT",Coord(8,10)},
        {"TAC",Coord(4,8)},
    });
    ASSERT_TRUE(board.insert_word("TAC",Place(8,8,Place::horizontal),uses));
    cout << board << endl;
    ASSERT_TRUE(run_board_test(board, test_set, true));
    cout << board << endl;
    }
}

TEST(board,fitting_002) {
    Board board(define_board("CACAT",{"CAT","TAC"}));
    cout << "board.debug=" << board.debug << endl;
    vector<const CharAtPos> uses;
    vector<pair<string,Coord>> test_set({
        {"TAC",Coord(8,8)},
        {"TAC",Coord(8,9)},
        {"TAC",Coord(8,10)},
        {"TAC",Coord(6,10)},
        {"TAC",Coord(8,10)},
        {"TAC",Coord(6,8)}
    });
    ASSERT_TRUE(board.insert_word("CAT",Place(8,8,Place::horizontal),uses));
    cout << board << endl;
    ASSERT_TRUE(run_board_test(board, test_set, true));
    cout << board << endl;
}

