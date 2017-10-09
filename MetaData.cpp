//
//  MetaData.cpp
//  OSAssignment
//
//  Created by Xiaolong Ma on 09/10/2017.
//  Copyright © 2017 TI. All rights reserved.
//

#include "MetaData.hpp"

//Load meta file data and return a vector including all metaData instances
vector<MetaData> loadMetaData(string filePath)
{
    vector<string> metadataStr;
    ifstream inFile;
    inFile.open(filePath);
    assert(inFile.is_open());
    string line;
    vector<MetaData> metaDatas;
    
    while (getline(inFile,line))
    {
        if (line!="Start Program Meta-Data Code:"&& line!="End Program Meta-Data Code.")
        {
            vector<string> tempStrs = SplitString(line, ";");
            if (tempStrs.size()>0) {
                for (int i = 0; i< tempStrs.size(); i++) {
                    if (tempStrs[i] != " ") {
                        metadataStr.push_back(removeUnnecessaryDelimiter(tempStrs[i]));
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < metadataStr.size(); i++) {
        MetaData metaData;
        metaData.fullName = removeUnnecessaryDelimiter(metadataStr[i]);
        vector<string> tempStrVect = split(metadataStr[i], "()");
        metaData.instructor = tempStrVect[0];
        metaData.action = tempStrVect[1];
        metaData.cycleNum = stoi(tempStrVect[2]);
        metaData.checkData();
        metaDatas.push_back(metaData);
    }
    return metaDatas;
}

