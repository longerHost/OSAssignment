//
//  MetaData.hpp
//  OSAssignment
//
//  Created by Xiaolong Ma on 09/10/2017.
//  Copyright © 2017 TI. All rights reserved.
//

#ifndef MetaData_hpp
#define MetaData_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

//Split function for string split by characters mutliple characters
vector<string> split(const string &s, const string &seperator);

//Split string by one charcter
vector<string> SplitString(const string &s, const string &c);

string removeUnnecessaryDelimiter(string s);

class MetaData
{
public:
    string fullName;
    string instructor;
    string action;
    int cycleNum;
    
public:
    void checkData()
    {
        vector<string> tempVector = split(fullName, "()");
        if (tempVector.size() != 3) {
            cout << "Meta-Data Error: " << fullName << '\n' << "Please check your Meta-Data file." << endl;
            exit(1);
        }
    };
};


#endif /* MetaData_hpp */
