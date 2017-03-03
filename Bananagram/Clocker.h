/*
 *  Clocker.h
 *  Simple timer wrapper
 *
 *  Created by Michah Lerner on 8/5/08.
 *  Copyright 2008. All rights reserved.
 *
 */
#ifndef __CLOCKER_H__
#define __CLOCKER_H__
#include <sys/time.h>
#include <sys/resource.h>
#include <sstream>
#include <iostream>
#include <ostream>
#include <iomanip>
#include <string>

using namespace std;

class Clocker {
    timeval start, stop;
    unsigned long usageStart, usageEnd;
    struct rusage r_usageStart, r_usageEnd;
    string since,
    now;
public:
    Clocker() :
    start(),
    stop(),
    usageStart(),
    usageEnd(),
    r_usageStart(),
    r_usageEnd(),
    since(),
    now() {
        begin();
    }
    virtual ~Clocker() { }
    void begin();
    void end();
    double getTimediff();
    unsigned long getUsage();
    string tod();
    string tod(string& data);
    string tod(const timeval& tv, string& data);
    string readReset();
    string readResetDetailed(const string& msg="");
};
#endif
