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
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <map>

using namespace std;

/*---  Time  --- */

//Get system time by second
long systemTimeBySecond();

//Get system time by microsecond
long systemTimeByUSecond();

//Get System time including second and microsecond
double systemRealTime();

//Get time interval
double timeInterval(double flagTime);

//Let the process waiting
void waitTime(double operationEndTime);

//Let the process wait for t seconds
void waitSeconds(int t);

//Let the process wait for t uSeconds
void waituSeconds(int t);

/*---  String ----*/

//Split function for string split by characters mutliple characters
vector<string> split(const string &s, const string &seperator);

//Split string by one charcter
vector<string> SplitString(const string &s, const string &c);

string removeUnnecessaryDelimiter(string s);

//Load content from file by line
vector<string> loadContentByLine(string filePath);

//Check if file name is legal
bool isFileNameLegal(string fileName);

//Allocate hard drive
int allocateHardDrive(int totalHD);

//Allocate printer
int allocatePrinter(int totalPrinter);

//Allocate memory
string allocateMemory(long int blockSize,long int totalMemory);

#endif /* Functions_hpp */
