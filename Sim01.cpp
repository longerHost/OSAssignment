//
//  main.cpp
//  kk
//
//  Created by Xiaolong Ma on 14/09/2017.
//  Copyright © 2017 TI. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <array>
#include <cassert>
#include <map>
#include <sstream>
#include "Functions.hpp"
#include "Process.hpp"
#include <iomanip>
#include <stdlib.h>

using namespace std;

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

string removeUnnecessaryDelimiter(string s)
{
    string::size_type pos = s.find_first_not_of(" ");
    string tempStr = s.substr(pos);
    
    if (tempStr.find(".") == tempStr.size()-1) {
        tempStr = tempStr.substr(0,tempStr.find("."));
    }
    return tempStr;
}

//Load content from file by line
vector<string> loadContentByLine(string filePath)
{
    vector<string> contents;
    ifstream inFile;
    inFile.open(filePath);
    assert(inFile.is_open());
    
    string s;
    while (getline(inFile, s)) {
        contents.push_back(s);
    }
    inFile.close();
    return contents;
}

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

//Load meta file data
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

void outputLog(Config config, vector<MetaData> metadata)
{
    //Print to screen
    cout<< "Configuration File Data" << endl;
    cout << "Processor = " << config.processor.cycle << " ms/cycle" << endl;
    cout << "Monitor = " << config.monitor.cycle  << " ms/cycle" << endl;
    cout << "Hard Drive = " << config.hardDrive.cycle << " ms/cycle" << endl;
    cout << "Printer = " << config.printer.cycle <<  " ms/cycle" << endl;
    cout << "Keyboard = " << config.keyboard.cycle << " ms/cycle" << endl;
    cout << "Memory = " << config.memory.cycle << " ms/cycle" << endl;
    cout << "Speaker = " << config.mouse.cycle << " ms/cycle" << endl;
    
    if (config.logtype == "Log to Both") {
        cout << "Logged to: monitor and " << config.logFilePath << endl;
    }else{
        cout << "Logged to: monitor" << endl;
    }
    cout << '\n' << endl;
    cout << "Meta-Data Metrics" << endl;
    
    vector<string> tempVect;
    
    for (int i = 2; i < metadata.size()-2; i++) {
        tempVect = split(metadata[i].fullName, "()");
        
        string tempAction = tempVect[1]; //Action instructor
        string tempCycle = tempVect.back(); //Cycle time
        
        int factor = 0;
        if (tempAction == "allocate" || tempAction == "block") {
            factor = config.memory.cycle;
        }else if(tempAction == "keyboard"){
            factor = config.keyboard.cycle;
        }else if (tempAction == "hard drive"){
            factor = config.hardDrive.cycle;
        }else if(tempAction == "printer"){
            factor = config.printer.cycle;
        }else if (tempAction == "monitor"){
            factor = config.monitor.cycle;
        }else if (tempAction == "run"){
            factor = config.processor.cycle;
        }else if (tempAction == "speaker"){
            factor = config.speaker.cycle;
        }else if (tempAction == "mouse"){
            factor = config.processor.cycle;
        }else if(tempAction == "end"){
            factor= 0;
        }else{
            cout << "Wrong input in Meta-Data" << endl;
            assert(factor);
        }
        int cycle = stoi(tempCycle);
        cout << metadata[i].fullName << " - " << cycle * factor << " ms" << endl;
    }
    
    //Print to file
    if (config.logtype == " Log to Both") {
        ofstream outFile("/Users/xiaolongma/Desktop/kk/output3.out");
        if (!outFile) {
            cout << "Open error while output" <<endl;
            exit(1);
        }
        outFile << "Configuration File Data"<< '\n' <<
        "Processor = " << config.processor.cycle << " ms/cycle" << '\n' <<
        "Monitor = " << config.monitor.cycle  << " ms/cycle" << '\n' <<
        "Hard Drive = " << config.hardDrive.cycle << " ms/cycle" << '\n' <<
        "Printer = " << config.printer.cycle <<  " ms/cycle" << '\n' <<
        "Keyboard = " << config.keyboard.cycle << " ms/cycle" << '\n' <<
        "Memory = " << config.memory.cycle << " ms/cycle" << '\n' <<
        "Speaker = " << config.mouse.cycle << " ms/cycle" << '\n' <<
        "Logged to: monitor and " << config.logFilePath << '\n' <<
        '\n' <<
        "Meta-Data Metrics" << endl;
        
        vector<string> tempVect;
        
        for (int i = 2; i < metadata.size() - 2; i++) {
            tempVect = split(metadata[i].fullName, "()");
            
            string tempAction = tempVect[1];
            string tempCycle = tempVect.back();
            
            int factor = 0;
            if (tempAction == "allocate" || tempAction == "block") {
                factor = config.memory.cycle;
            }else if(tempAction == "keyboard"){
                factor = config.keyboard.cycle;
            }else if (tempAction == "hard drive"){
                factor = config.hardDrive.cycle;
            }else if(tempAction == "printer"){
                factor = config.printer.cycle;
            }else if (tempAction == "monitor"){
                factor = config.monitor.cycle;
            }else if (tempAction == "run"){
                factor = config.processor.cycle;
            }else if (tempAction == "speaker"){
                factor = config.speaker.cycle;
            }else if (tempAction == "mouse"){
                factor = config.processor.cycle;
            }else{
                cout << "Wrong input in Meta-Data" << endl;
                assert(factor);
            }
            int cycle = stoi(tempCycle);
            outFile << metadata[i].fullName << " - " << cycle * factor << " ms" << endl;
        }
        
        outFile.close();
        cout<< "log to Both" << endl;
    }
}

void waitTime(double operationEndTime)
{
    while (systemRealTime()< operationEndTime) {};
}

int timeOfOperation(Config config,MetaData metadata)
{
    string tempAction = metadata.action;
    
    int factor = 0;
    if (tempAction == "allocate" || tempAction == "block") {
        factor = config.memory.cycle;
    }else if(tempAction == "keyboard"){
        factor = config.keyboard.cycle;
    }else if (tempAction == "hard drive"){
        factor = config.hardDrive.cycle;
    }else if(tempAction == "printer"){
        factor = config.printer.cycle;
    }else if (tempAction == "monitor"){
        factor = config.monitor.cycle;
    }else if (tempAction == "run"){
        factor = config.processor.cycle;
    }else if (tempAction == "speaker"){
        factor = config.speaker.cycle;
    }else if (tempAction == "mouse"){
        factor = config.processor.cycle;
    }else if(tempAction == "end"){
        factor= 0;
    }else if(tempAction == "start"){
        factor = 0;
    }else{
        cout << "Wrong input in Meta-Data" << endl;
        assert(factor);
    }
    
    int timeOfOperation = factor * metadata.cycleNum;
    
    return timeOfOperation;
}

double getOperationTimeInterval(double flagTime)
{
    return systemRealTime() - flagTime;
}

void outPutByMetaData(double flagTime,MetaData metadata)
{
    //Program Start
    if (metadata.instructor == "S") {
        if (metadata.action == "start") {
            cout<< setiosflags(ios::fixed) << timeInterval(flagTime) << " - Simulator Program starting" << endl;
        }else if(metadata.action == "end"){
            cout<< setiosflags(ios::fixed) << timeInterval(flagTime) << " - Simulator Program ending" << endl;
        }else{
            cout << "Wrong file format" << endl;
        }
    }
    
    //Process Start
    if (metadata.instructor == "A") {
        //Operation start and end
        if (metadata.action == "start") {
            cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << "OS: preparing process 1" <<endl;
            
            cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << "OS: starting process 1" <<endl;
        }else if (metadata.action == "end"){
            cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << "OS: removing process 1" <<endl;
        }
    }
}

void outputLogSim2(Config config, vector<MetaData> metadata)
{
    //System start time
    double startTime = systemRealTime();
    cout << setiosflags(ios::fixed) << systemRealTime() - startTime << "- Simulator Program starting" << endl;
    
    queue<MetaData> metaDataQueue;
    for (int i = 0 ; i< metadata.size(); i++) {
        metaDataQueue.push(metadata[i]);
    }
    
    //out put each
    while (!metaDataQueue.empty()) {
        
        MetaData md = metaDataQueue.front();
        cout<< md.fullName << endl;
        string workStr;
        string operationStartStr;
        string operationEndStr;
        
        if (md.instructor == "S") {
            workStr = " OS: ";
        }else{
            workStr = " Process 1: ";
        }
        
        if (md.action == "start") {
            operationStartStr = "start processing action";
        }else{
            operationStartStr = "TBD ";
        }
        
        if (md.action == "start") {
            operationEndStr = "end processing action";
        }else{
            operationEndStr = "TBD ";
        }
        
        cout<< systemRealTime() - startTime << workStr << operationStartStr << endl;
        
        double timeIntervalInSeconds = timeOfOperation(config, md)/1000;
        
        waitTime(systemRealTime() + timeIntervalInSeconds);
        
        cout<< systemRealTime() - startTime << workStr << operationEndStr << endl;

        metaDataQueue.pop();
    }
}

//main function
int main(int argc, const char * argv[]) {
    
    //Get config instance by file path
    Config config = loadConfigData("/Users/xiaolongma/Desktop/OSAssignment/config_1-1.conf");
//    Config config = loadConfigData(argv[1]);

    //Get all metaData
//    vector<MetaData> metadata = loadMetaData(config.filePath);
      vector<MetaData> metadata = loadMetaData("/Users/xiaolongma/Desktop/kk/test_1a.mdf");

    //Output log
//    outputLog(config,metadata);
    
    outputLogSim2(config, metadata);
    
    return 0;
}

