//
//  Functions.cpp
//  kk
//
//  Created by Xiaolong Ma on 08/10/2017.
//  Copyright © 2017 TI. All rights reserved.
//

#include "Functions.hpp"

//Get system time by second
long systemTimeBySecond()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    __darwin_time_t second = tv.tv_sec;
    return second;
};

//Get system time by microsecond
long systemTimeByUSecond()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    __darwin_time_t second = tv.tv_usec;
    return second;
};

//Get System time including second and microsecond
double systemRealTime()
{
    long systemSecond = systemTimeBySecond();
    long systemuSecond = systemTimeByUSecond();
    double systemRealTime = systemSecond + (double)systemuSecond/1000000;
    
    return systemRealTime;
};

double timeInterval(double flagTime)
{
    return systemRealTime() - flagTime;
};

