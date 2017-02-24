//
//  simple_utils.cpp
//  simi
//
//  Created by Michah Lerner on 12/29/13.
//  Copyright (c) 2013 Michah Lerner. All rights reserved.
//

#include <algorithm>
#include <fstream>
#include <sys/stat.h>
#include <iterator>
#include <libgen.h>
#include <unistd.h>
#include <chrono>
#include <random>
#include <iomanip>

#include "simple_utils.h"
#include "Board.h"
#include "Place.h"

using std::string;
using std::vector;
using std::list;
using std::deque;
using std::allocator;

std::ostream& operator<<(std::ostream& out, const vector<char>& vec) {
    copy(vec.cbegin(), vec.cend(), ostream_iterator<char>(out));
    return out;
}
std::ostream& operator<<(std::ostream& out, const vector<const Place>& vec) {
    copy(vec.cbegin(), vec.cend(), ostream_iterator<const Place>(out));
    return out;
}
std::ostream& operator<<(std::ostream& out, const deque<const Coord>& vec) {
    for(deque<const Coord>::const_iterator it = vec.cbegin(); it != vec.cend(); it++) out << *it << "; ";
    return out;
}
std::ostream& operator<<(std::ostream& out, const vector<vector<char>>& vec) {
    for(vector<vector<char>>::const_iterator it = vec.cbegin(); it != vec.cend(); it++) out << *it << endl;
    return out;
}
std::ostream& operator<<(std::ostream& out, const deque<const Place>& vec) {
    for(deque<const Place>::const_iterator it = vec.cbegin(); it != vec.cend(); it++) out << *it << "; ";
    return out;
}
std::ostream& operator<<(std::ostream& out, const vector<const char_at_pos>& vec) {
    for(const char_at_pos &c : vec) out << c << " ";
    return out;
}
std::ostream& operator<<(std::ostream& out, const vector<const Coord>& vec) {
    for(auto c : vec) out << c;
    return out;
}
std::ostream& operator<<(std::ostream& out, const vector<Coord>& vec) {
    for(auto c : vec) out << c;
    return out;
}
std::ostream& operator<<(std::ostream& out, const char_at_pos& cap) {
    out << cap.first << "@" << cap.second;
    return out;
}
std::ostream& operator<<(std::ostream& out, const Coord& coord) {
    out << "(" << coord.first << "," << coord.second << ") ";
    return out;
}
std::ostream& operator<<(std::ostream& out, Coord& coord) {
    out << "(" << coord.first << "," << coord.second << ") ";
    return out;
}
std::ostream& operator<<(std::ostream& out, const Place& place) {
    out << place.row
    << ":" << place.col
    << ":" << Place::PlaceNames[place.direction] << " ";
    return out;
}

std::ostream& operator<<(std::ostream& out, const Board& board) {
    if(!board.empty())
        {
        int from_row = board.first_nonempty_row();
        int to_row = board.last_nonempty_row();
        int from_col = board.first_nonempty_col();
        int to_col = board.last_nonempty_col();
        for(int i = from_row; i <= to_row; i++) {
            for(int j = from_col; j <= to_col; j++) {
                out << board.board[i][j];
            }
            out << endl;
        }
        }
    return out;
}

int
fexist(const std::string & name)
{
    struct stat buffer;
    return stat(name.c_str(), &buffer) == 0 ? 1 : 0;
}

off_t
fsize(const std::string & name)
{
    struct stat buffer;
    stat(name.c_str(), &buffer);
    return buffer.st_size;
}

off_t
fsize(int fd) {
    struct stat buffer;
    return fstat(fd,&buffer)==0?buffer.st_size:0;
}

std::string
unique_name(const std::string & base)
{
    int cnt = 0;
    std::string name(base);
    while(fexist(name))
        name = base + "." + std::to_string(cnt++);
    return name;
}

string& toupper(std::string &s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return s;
}

template <template <typename,typename> class C=std::list, typename E=std::string, typename A=std::allocator<std::string>>
void
rvec(const std::string & name, C<E,A> &dat)
{
    if(fexist(name)) {
        
        std::ifstream file(name);
        file.setf(std::ios_base::uppercase);
#if 1
        std::copy(std::istream_iterator<E>(file), std::istream_iterator<E>(), std::back_inserter(dat));
#else
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::istringstream iss(content);
        std::copy(std::istream_iterator<E>(iss), std::istream_iterator<E>(), std::back_inserter(dat));
#endif
    }
}

template <class T>
std::string
tobin(T v)
{
    std::string res;
    for(int i=8*sizeof(T)-1; i>=0; i--)
        if(v & 0x1<<i) res.push_back('1');
        else if(res.length()) res.push_back('0');
    while(res.length()<8) res="0"+res;
    return res;
}

void
upperCase(string& in) {
    for(string::iterator p = in.begin(); p != in.end(); p++)
        *p = toupper(*p);
}

void trim(string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (string::npos == start) {
        str.erase(str.begin(),str.end());
        return;
    }
    size_t end = str.find_last_not_of(" \t\r\n");
    str.erase(str.begin(),str.begin()+start);
    str.erase(str.end()-end,str.end());
}


std::string tod()
{
    char buffer[80];
    time_t t = time(NULL);
    tm *tm = localtime(&t);
    strftime(buffer,80,"%m-%d-%Y %X %Z: ", tm);
    return std::string(buffer);
}


std::string
getBasename(const std::string& path, const std::string& suffix)
{
    size_t len = path.length() - suffix.length();
    char *pathc = strndup(path.c_str(), len);
    std::string answer(basename(pathc));
    free(pathc);
    return answer;
}

std::string
getDirname(const std::string& path)
{
    char *dirc = strdup(path.c_str());
    std::string answer(dirname(dirc));
    free(dirc);
    return answer;
}

std::string
getCwd()
{
    char *wd=getcwd(NULL, 0);
    std::string res(wd);
    free(wd);
    return res;
}

using std::cout;
using std::endl;

void showargs(int argc, char * const argv[]) {
    cout << "running from " << getCwd() << "\n";
    for(int i=0;i<argc;i++)
        cout << argv[i] << " ";
    cout << endl;
}

static /*const*/ std::vector<ushort> initial_counts({
    13, 3, 3, 6, 18,
    3, 4, 3, 12, 2,
    2, 5, 3, 8, 11,
    3, 2, 9, 6, 9,
    6, 3, 3, 2, 3,
    2
});

vector<char> initialize_tiles() {
    vector<char> tiles;
    for(unsigned long i = 0; i < initial_counts.size(); i++) {
        for(unsigned long j = 0; j < initial_counts[i]; j++)
            tiles.push_back('A'+i);
    }
    unsigned long long seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(tiles.begin(), tiles.end(), std::mt19937_64(seed));
    cout << "There are " << tiles.size() << " initial tiles." << endl;
    return tiles;
}

vector<char> tochar(const string& str) {
    vector<char> res;
    for(char ch : str) res.push_back(ch);
    return res;
}

template void rvec(const string & name, list<string> & dat);
template void rvec(const string & name, vector<string> & dat);

//template void readvec<std::string>(const std::string & name, std::list<std::string> &dat);
//template void rvec<vector, string, allocator<string>>(const string & name, vector<string> & dat);
//template void rvec<vector>(const string & name, vector<string> & dat);
