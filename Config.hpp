//
//  Config.hpp
//  OSAssignment
//
//  Created by Xiaolong Ma on 09/10/2017.
//  Copyright © 2017 TI. All rights reserved.
//

#ifndef Config_hpp
#define Config_hpp

#include <stdio.h>
#include "Functions.hpp"

class Device
{
public:
    string name;
    int cycle;
};

class Config
{
public:
    string versionNum;
    string filePath;
    Device processor;
    Device monitor;
    Device hardDrive;
    Device printer;
    Device keyboard;
    Device memory;
    Device mouse;
    Device speaker;
    string logtype;
    string logFilePath;
};

//Load configuration file data and return a config instance which contains all config file info
Config loadConfigData(string filePath);

#endif /* Config_hpp */
