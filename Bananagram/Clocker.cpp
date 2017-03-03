/*
 *  Clocker.cpp
 *  Simple timer wrapper
 *
 *  Created by Michah Lerner on 8/5/08.
 *  Copyright 2008. All rights reserved.
 *
 */

#include "Clocker.h"
extern int errno;

void Clocker::begin() {
    now.clear();
    since.clear();
    gettimeofday(&start, NULL);
    usageStart = clock();
    if(getrusage(RUSAGE_SELF,&r_usageStart)!=0)
        cerr << "Error " << errno << " on Clocker::begin()" << endl;
}

void Clocker::end() {
    if(getrusage(RUSAGE_SELF,&r_usageEnd)!=0)
        cerr << "Error " << errno << " on Clocker::end()" << endl;
    gettimeofday(&stop, NULL);
    usageEnd = clock();
    tod(since);
    tod(stop,now);
}

double Clocker::getTimediff() {
    long dsec = stop.tv_sec - start.tv_sec;
    long usec = stop.tv_usec - start.tv_usec;
    return (1000000 * dsec + usec) / 1E6;
}

unsigned long Clocker::getUsage() {
    unsigned long diff = usageEnd - usageStart;
    return diff;
}

string Clocker::tod() {
    return tod(now);
}

string Clocker::tod(string& data) {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tod(tv,data);
}

string Clocker::tod(const timeval& tv, string& result) {
    char buffer[64];
    strftime(buffer, 63, "%F %T", localtime(&tv.tv_sec));
    ostringstream ostr;
    ostr << buffer << '.' << setfill('0') << setw(6) << tv.tv_usec;
    result.assign(ostr.str());
    return result;
}

string Clocker::readReset() {
    ostringstream str;
    end();
    double diff = getTimediff();
    double timeUsed = getUsage() / 1E6;
    str << ":: " << tod(now) << " :: " << setfill('0') << setprecision(6) << setw(10) << diff << " sec elapsed; "
    << setfill(' ') << setprecision(4) << timeUsed << " cpu sec used" ;
    begin();
    return str.str();
}

string Clocker::readResetDetailed(const string& msg) {
    ostringstream str;
    long dsec, usec;
    double utime, stime;
    end();
    double diff = getTimediff();
    unsigned long usage = getUsage();
    double timeUsed = usage / 1E6;

    dsec = r_usageEnd.ru_utime.tv_sec - r_usageStart.ru_utime.tv_sec;
    usec = r_usageEnd.ru_utime.tv_usec - r_usageStart.ru_utime.tv_usec;
    utime = (1000000* dsec + usec) / 1E6;

    dsec = r_usageEnd.ru_stime.tv_sec - r_usageStart.ru_stime.tv_sec;
    usec = r_usageEnd.ru_stime.tv_usec - r_usageStart.ru_stime.tv_usec;
    stime = (1000000* dsec + usec) / 1E6;

    long maxrss = r_usageEnd.ru_maxrss;
    long nvcsw = r_usageEnd.ru_nvcsw ;
    long nivcsw = r_usageEnd.ru_nivcsw;

    str << msg << ":: " << now << " :: " << setfill('0') << setprecision(6) << setw(10) << diff << " sec elapsed";
    str << " (since " << since << ")" << endl;
    str << msg << ":: " << now << " :: time(user) " << setfill(' ') << setprecision(4) << utime;
    str << " :: time(system) " << setfill(' ') << setprecision(4) << stime;
    str << " :: time(total) " << setfill(' ') << setprecision(4) << timeUsed << endl;
    str << msg << ":: " << now << " :: maxRss=" << maxrss << " :: cntxSwitch(user)=" << nvcsw << " :: cntxSwitch(forc)=" << nivcsw;

    begin();
    return str.str();
}
