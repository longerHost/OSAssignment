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

//Total time of each operation(millisecond)
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

//Output each line
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

//Get process by metadatas
queue<Process> creatProcessByMetadatas(vector<MetaData> metadatas)
{
    //To obtain processes
    queue<Process> processQue;
    
    int processStartNum = 0;
    int processEndNum = 0;
    
    //count total process number
    int processNum = 0;
    
    //This queue is the container of all instructions
    queue<MetaData> metaLongQueue;
    
    //Check if the file format is right
    for (int i = 0; i<metadatas.size(); i++) {
        MetaData md = metadatas[i];
        if (md.instructor == "A") {
            if (md.action == "start") {
                processStartNum++;
            }
            
            if (md.action == "end") {
                processEndNum++;
            }
        }
        metaLongQueue.push(md);
    }
    
    //make sure the file contains equal A(Start) and A(end)
    if (processStartNum != processEndNum)
    {
        cout<< "Wrong file format:instuctions A(start)0 not equals to A(end)" <<endl;
        exit(1);
    }
    
    //Separate long Queue by process and put the process instance to processQue
    queue<MetaData> metaProcessQueue;
    while (!metaLongQueue.empty()) {
        
        MetaData md = metaLongQueue.front();
        
        if (md.instructor == "A" && md.action == "start") {
            //Empty metaProcessQueue
            while (!metaProcessQueue.empty()) {
                metaProcessQueue.pop();
            }
        }
        
        metaProcessQueue.push(md);
        
        if (md.instructor == "A" && md.action == "end") {
            Process nProcess;
            nProcess.processID = ++processNum;
            nProcess.processState = START;
            nProcess.metaDataQueue = metaProcessQueue;
            processQue.push(nProcess);
        }
        
        metaLongQueue.pop();
    }
    
    return processQue;
}

void outputLogSim2(Config config, vector<MetaData> metadatas)
{
    //System start time
    double startTime = systemRealTime();
    //S(Start)0 system start
    cout << setiosflags(ios::fixed) << systemRealTime() - startTime << "- Simulator Program starting" << endl;
    
    /*** application running ***/
    queue<Process> processQue = creatProcessByMetadatas(metadatas);
    
    while (!processQue.empty()) {
        queue<MetaData> metaQue = processQue.front().metaDataQueue;
        while (!metaQue.empty()) {
            
            MetaData md = metaQue.front();
            
            outPutByMetaData(startTime, config, md);
            
            metaQue.pop();
        }
        processQue.pop();
    }
    
    //S(end)0 system end
    cout << setiosflags(ios::fixed) << systemRealTime() - startTime << " - Simulator Program ending" << endl;
    
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

