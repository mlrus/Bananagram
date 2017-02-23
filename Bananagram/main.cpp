//
//  main.cpp
//  bananagram
//
//  Created by Michah Lerner on 2/7/17.
//  Copyright © 2017 Michah Lerner. All rights reserved.
//

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

void usage() {
    cout << "Options\n";
    cout << "   -?          ! help\n";
    cout << "   -h          ! help\n";
    cout << "   -d          ! debug\n";
    cout << "   -D ##       ! dimension\n";
    cout << "   -n ##       ! draw n tiles\n";
    cout << "   -F ##       ! dictionary filename\n";
    cout << "   -L letters  ! initial letters\n";
    cout << "   -P          ! preserve order of dictionary\n";
    cout << "   -p          ! do not preserve order or dictionary [default]\n";
}

int dim=256, tile_count=0;
bool debug = false;
bool preserve_order = true;
string dict_filename;
string initial_letters; // ("BASEDHABTEDIALFOROO");

int parseargs(int argc, char * const argv[]) {
  
    int ch;
    while ((ch = getopt(argc, argv, "?hdD:n:F:L:pP")) != -1) {
        switch(ch) {
            case '?':
            case 'h': usage();
                return 1;
            case 'd': debug = true;
                break;
            case 'D': dim=atoi(optarg);
                break;
            case 'n': tile_count=atoi(optarg);
                break;
            case 'F': dict_filename.assign(optarg);
                break;
            case 'L': initial_letters.assign(optarg);
                break;
            case 'P': preserve_order = true;
                break;
            case 'p': preserve_order = false;
                break;
            default:  cout << "Use: '" << argv[0] << "-?' for help\n";
                return -1;
        }
    }
    return 0;
}

int main(int argc,  char * const argv[]) {
    showargs(argc, argv);
    int rc = parseargs(argc, argv);
    if(rc!=0) return rc;
    cout << "debug="<<boolalpha<<debug<<"; dim="<<dim<<"; tile_count="<<tile_count<<endl;
    cout << "dictionary="<<dict_filename<<"; initial_letters="<<initial_letters<<endl;
    
    Dictionary dictionary;
    if(!dict_filename.empty())
        dictionary.load(dict_filename);
    else
        dictionary.add_words({"IF","AFT","ALOES","TEAR",
            "HIT","DO","FOE","BARD","DO","BASH","JOT",
            "WARM","CAT","MAT","APPLE","PIE"});
    
    Board board(dictionary, initialize_tiles(), dim, tile_count);
    board.debug = debug;
    
    if(initial_letters.empty())
        board.peel(tile_count);
    else
        if(!board.peel(tochar(initial_letters))) {
            cout << "Cannot assign " << initial_letters << endl;
            return 1;
        }
    
    if(debug)
        dictionary.dump(cout);
    
    const Place starts[] = {
        Place(dim/2, dim/2, Place::Direction::horizontal),
        Place(dim/2, dim/2, Place::Direction::vertical)
    };
    
    int numanswers = 0;
    vector<const char_at_pos> uses;
    string unplayed(board.show_unplayed());
    cout << board << endl;
    for(string & word : dictionary.words) {
        for(const Place start_at : starts) {
            if(board.insert_word(word, start_at, uses)) {
                deque<const Coord> expand_from;
                for(auto cap : uses)
                    expand_from.push_back(cap.second);
                if(board.debug)cout << "\nExpand from\n" << expand_from << endl;
                bool result = board.newsolve(expand_from);
                if(result) numanswers++;
                board.revert(uses);

            }
        }
    }
    cout << "Computed " << numanswers << " results for: " << unplayed << endl;
    
    return 0;
}
