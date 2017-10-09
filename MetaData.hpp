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
#include "Functions.hpp"

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

//Load meta file data and return a vector including all metaData instances
vector<MetaData> loadMetaData(string filePath);

#endif /* MetaData_hpp */
