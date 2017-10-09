//
//  Config.cpp
//  OSAssignment
//
//  Created by Xiaolong Ma on 09/10/2017.
//  Copyright © 2017 TI. All rights reserved.
//

#include "Config.hpp"

//Load configuration file data
Config loadConfigData(string filePath)
{
    Config config;
    vector<string> configInfo;
    map<string, string> configMap;
    
    configInfo = loadContentByLine(filePath);
    
    //Split the configInfo string with ":"
    for (int i = 0; i < configInfo.size(); i++) {
        string tempStr = configInfo[i];
        string key,value;
        if (tempStr != "Start Simulator Configuration File" && tempStr != "End Simulator Configuration File")
        {
            vector<string> splitedVec = split(tempStr, ":");
            if (splitedVec.size() != 2) {
                cout<< "Wrong Configuration file format" << endl;
            }
            key = removeUnnecessaryDelimiter(splitedVec[0]);
            value = removeUnnecessaryDelimiter(splitedVec[1]);
        }
        configMap.insert(make_pair(key, value));
    }
    
    //Get config details
    map<string, string>::iterator configIter;
    
    configIter = configMap.find("Version/Phase");
    config.versionNum = configIter->second;
    
    configIter = configMap.find("File Path");
    config.filePath = configIter->second;
    
    configIter = configMap.find("Processor cycle time (msec)");
    config.processor.name = configIter->first;
    config.processor.cycle =stoi(configIter->second);
    
    configIter = configMap.find("Monitor display time (msec)");
    config.monitor.name = configIter->first;
    config.monitor.cycle =stoi(configIter->second);
    
    configIter = configMap.find("Hard drive cycle time (msec)");
    config.hardDrive.name = configIter->first;
    config.hardDrive.cycle =stoi(configIter->second);
    
    configIter = configMap.find("Printer cycle time (msec)");
    config.printer.name = configIter->first;
    config.printer.cycle =stoi(configIter->second);
    
    configIter = configMap.find("Keyboard cycle time (msec)");
    config.keyboard.name = configIter->first;
    config.keyboard.cycle =stoi(configIter->second);
    
    configIter = configMap.find("Memory cycle time (msec)");
    config.memory.name = configIter->first;
    config.memory.cycle =stoi(configIter->second);
    
    configIter = configMap.find("Mouse cycle time (msec)");
    config.mouse.name = configIter->first;
    config.mouse.cycle =stoi(configIter->second);
    
    configIter = configMap.find("Speaker cycle time (msec)");
    config.speaker.name = configIter->first;
    config.speaker.cycle =stoi(configIter->second);
    
    configIter = configMap.find("Log");
    config.logtype = configIter->second;
    
    configIter = configMap.find("Log File Path");
    config.logFilePath = configIter->second;
    
    return config;
}

