//
//  main.cpp
//  kk
//
//  Created by Xiaolong Ma on 14/09/2017.
//  Copyright © 2017 TI. All rights reserved.
//

#include <iostream>
#include <array>
#include <sstream>
#include "Functions.hpp"
#include "Process.hpp"
#include "MetaData.hpp"
#include "Config.hpp"
#include <iomanip>
#include <stdlib.h>

using namespace std;

//Output of Assignment 1
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


void outPutByMetaData(double flagTime,Config config,MetaData metadata)
{
    //System Start
    if (metadata.instructor == "S") {
        if (metadata.action == "start") {
            cout<< setiosflags(ios::fixed) << timeInterval(flagTime) << " - Simulator Program starting" << endl;
        }else if(metadata.action == "end"){
            cout<< setiosflags(ios::fixed) << timeInterval(flagTime) << " - Simulator Program ending" << endl;
        }else{
            cout << "Wrong file format" << endl;
        }
    }
    
    //Application Start
    if (metadata.instructor == "A") {
        //Operation start and end
        if (metadata.action == "start") {
            cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << "OS: preparing process 1" <<endl;
            
            cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << "OS: starting process 1" <<endl;
        }else if (metadata.action == "end"){
            cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << "OS: removing process 1" <<endl;
        }
    }
    
    //Process start
    string areaStr;
    string actionStartStr;
    string actionEndStr;
    
    if (metadata.instructor == "P" && metadata.action == "run") {
        areaStr = "Process 1:";
        actionStartStr = " start process action";
        actionEndStr = " end process action";
    }
    
    if (metadata.instructor == "M") {
        if (metadata.action == "allocate") {
            areaStr = "Process 1:";
            actionStartStr = " allocating memory";
            actionEndStr = " memory allocated at 0x0000000"; //TODO
        }
        
        if (metadata.action == "block") {
            areaStr = "Process 1:";
            actionStartStr = " start memory blocking";
            actionEndStr = " end memory blocking";
        }
    }
    
    if (metadata.instructor == "O" || metadata.instructor == "I") {
        string IOString;
        if (metadata.instructor == "O") {
            IOString = "output";
        }else{
            IOString = "input";
        }
        
        areaStr = "Process 1:";
        actionStartStr = " start " + metadata.action + " " + IOString;
        actionEndStr = " End " + metadata.action + " " + IOString;
    }
    
    if (metadata.instructor != "S" && metadata.instructor !="A") {
        cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << areaStr + actionStartStr <<endl;
        
        //Let the process running
        double timeIntervalInSeconds = timeOfOperation(config, metadata)/1000.0;
        waitTime(systemRealTime() + timeIntervalInSeconds);

        cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << areaStr + actionEndStr <<endl;
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
        
        outPutByMetaData(startTime, config, md);
        
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

