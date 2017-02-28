//
//  main.cpp
//  bananagram
//
//  Created by Michah Lerner on 2/7/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

/*
 Build: make banana
 Run:   ./banana -W IF,AFT,ALOES,TEAR,HIT,DO,FOE,BARD,DO,BASH,JOT -L BASEDHABTEDIALFOROO -o 4
 Help:  ./banana -h
 */

#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <ostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "defs.h"
#include "simple_utils.h"
#include "Dictionary.h"
#include "Board.h"
#include "Place.h"

extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;

using std::cout;
using std::endl;

void usage(char *cmd) {
    cout << "Usage\n";
    cout << "  " << cmd << " -W cat,hat,hit,pat,sat,sit,vat -L SAITHAT -P\n";
    cout << "Options\n";
    cout << "   -?            ! help\n";
    cout << "   -h            ! help\n";
    cout << "   -d            ! debug\n";
    cout << "   -D ##         ! dimension\n";
    cout << "   -n ##         ! draw n tiles\n";
    cout << "   -F ##         ! dictionary filename\n";
    cout << "   -W word,...   ! comma separated words\n";
    cout << "   -L letters    ! initial letters\n";
    cout << "   -M maxresults ! max results (default 1000)\n";
    cout << "   -P            ! preserve order of dictionary (otherwise sorts, and maybe shuffle)\n";
    cout << "   -p            ! do not preserve order or dictionary [default]\n";
    cout << "   -S            ! shuffle words in dictionary [default]\n";
    cout << "   -s            ! sort words in dictionary by length and alphabet\n";
    cout << "   -T            ! randomize tiles\n";
    cout << "   -t            ! do not randomize tiles\n";
    cout << "   -o #          ! output options (001=normal|010=debug|100=machine)\n";
}

int dim=256,
    tile_count=21,
    output_options = 1,
    max_results = 1000;
bool debug = false,
    preserve_order = false,
    shuffle_words = false,
    shuffle_t = false;
string dict_filename;
string dict_words;      // IF,AFT,ALOES,TEAR,HIT,DO,FOE,BARD,DO,BASH,JOT
string initial_letters; // BASEDHABTEDIALFOROO
int parseargs(int argc, char * const argv[]) {
    int ch;
    while ((ch = getopt(argc, argv, "?hdD:n:F:L:M:o:pPsStTW:")) != -1) {
        switch(ch) {
            case '?':
            case 'h': usage(argv[0]);
                return 1;
            case 'd': debug = true;
                break;
            case 'D': dim=atoi(optarg);
                break;
            case 'n': tile_count=atoi(optarg);
                break;
            case 'F': dict_filename.assign(optarg);
                break;
            case 'L': initial_letters.assign(optarg);  //TENUEFMDWRIISTLPONEOI
                break;
            case 'M': max_results=atoi(optarg);
                break;
            case 'o': output_options=atoi(optarg);
                break;
            case 'P': preserve_order = true;
                break;
            case 'p': preserve_order = false;
                break;
            case 'S': shuffle_words = true;
                break;
            case 's': shuffle_words = false;
                break;
            case 'T': shuffle_t = true;
                break;
            case 't': shuffle_t = false;
                break;
            case 'W': dict_words.assign(optarg);
                break;
            default:  cout << "Use: '" << argv[0] << "-?' for help\n";
                return -1;
        }
    }
    return 0;
}

void split(const string& s, char c,
           vector<string>& v) {
    string::size_type i = 0;
    string::size_type j = s.find(c);
    
    while (j != string::npos) {
        v.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);
        
        if (j == string::npos)
            v.push_back(s.substr(i, s.length()));
    }
}

int main(int argc,  char * const argv[]) {
    showargs(argc, argv);
    int rc = parseargs(argc, argv);
    if (rc!=0) return rc;
    cout << "debug=" << boolalpha << debug
         << "; dim=" << dim
         << "; tile_count=" << tile_count
         << "; output_options=" << output_options
         << "\ndictionary=" << dict_filename
         << "; initial_letters=" << initial_letters << endl;
    
    Dictionary dictionary(preserve_order,shuffle_words);
    if(!dict_filename.empty())
        dictionary.load(dict_filename);
    if(!dict_words.empty()) {
        vector<string> words;
        split(dict_words,',',words);
        dictionary.add_words(words);
    }

    vector<char> tiles(initialize_tiles());
    if(shuffle_t) {
        cout << "Randomizing unplaced tiles\n";
        shuffle_tiles(tiles);
    } else
        cout << "Not randomizing unolaced tiles\n";
    
    Board board(dictionary, tiles, dim, tile_count);
    board.debug = debug;
    board.output_options=output_options;
    board.max_results = max_results;
    
    if(initial_letters.empty())
        board.peel(tile_count);
    else
        if(!board.peel(tochar(initial_letters))) {
            cout << "Cannot assign " << initial_letters << endl;
            return 1;
        }
    
    const Place start = Place(dim/2, dim/2, Place::Direction::horizontal);

    int numanswers = 0;
    vector<const CharAtPos> uses;
    string unplayed(board.show_unplayed());
    cout << unplayed << endl;

    for(string & word : dictionary.words) {
        if(board.insert_word(word, start, uses)) {
            deque<const Coord> expand_from;
            for(auto cap : uses)
                expand_from.push_back(cap.coord);
            bool result = board.newsolve(expand_from);
            if (!result)
                cout << "No answer for: " << word << "\n";
            else
                numanswers++;
            board.revert(uses);
        }
    }

    for(auto kv : board.board_counts) {
        cout << kv.second << " : " << kv.first;
    }
    
    cout << "#unique=" << board.boards_seen.size()<<"; "
    << "#computed=" << board.numresults<<"; "
    << "#starts=" << numanswers
    << " for " << unplayed << endl;
 
    return 0;
}
