//
//  Functions.hpp
//  kk
//
//  Created by Xiaolong Ma on 08/10/2017.
//  Copyright © 2017 TI. All rights reserved.
//

#ifndef Functions_hpp
#define Functions_hpp

#include <stdio.h>
#include <sys/time.h>

//Get system time by second
long systemTimeBySecond();

//Get system time by microsecond
long systemTimeByUSecond();

//Get System time including second and microsecond
double systemRealTime();

//Get time interval
double timeInterval(double flagTime);

#endif /* Functions_hpp */
