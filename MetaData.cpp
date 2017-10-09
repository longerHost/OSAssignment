//
//  MetaData.cpp
//  OSAssignment
//
//  Created by Xiaolong Ma on 09/10/2017.
//  Copyright © 2017 TI. All rights reserved.
//

#include "MetaData.hpp"

//Split function for string split by characters mutliple characters
vector<string> split(const string &s, const string &seperator)
{
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;
    
    while(i != s.size()){
        int flag = 0;
        while(i != s.size() && flag == 0){
            flag = 1;
            for(string_size x = 0; x < seperator.size(); ++x)
            {
                if(s[i] == seperator[x]){
                    ++i;
                    flag = 0;
                    break;
                }
            }
        }
        
        flag = 0;
        string_size j = i;
        while(j != s.size() && flag == 0){
            for(string_size x = 0; x < seperator.size(); ++x)
                if(s[j] == seperator[x]){
                    flag = 1;
                    break;
                }
            if(flag == 0)
                ++j;
        }
        
        if(i != j){
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }
    return result;
}

//Split string by one charcter
vector<string> SplitString(const string &s, const string &c)
{
    string::size_type pos1, pos2;
    vector<string> v;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
        
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
    return v;
}

string removeUnnecessaryDelimiter(string s)
{
    string::size_type pos = s.find_first_not_of(" ");
    string tempStr = s.substr(pos);
    
    if (tempStr.find(".") == tempStr.size()-1) {
        tempStr = tempStr.substr(0,tempStr.find("."));
    }
    return tempStr;
}