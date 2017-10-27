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

//Pthreads
pthread_t thread;
pthread_attr_t attr;

//Thread runner function
void *ThreadProcessing(void *t) {
    long cycles=*((long*)(&t));
    double timeInSecond = cycles/1000000.0;
    waitTime(timeInSecond);
    pthread_exit(NULL);
}

//Output each line
void executeProcess(double flagTime,Config config,Process process)
{
    
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    while (!process.metaDataQueue.empty()) {
        
        MetaData metadata = process.metaDataQueue.front();
        
        stringstream ss;
        ss << process.processID;
        string processID = ss.str();
        
        //Application Start
        if (metadata.instructor == "A") {
            //Operation start and end
            if (metadata.action == "start") {
                
                //Change process state
                process.processState = START;
                
                cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << "OS: preparing process " + processID <<endl;
                
                cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << "OS: starting process " + processID <<endl;
                
                process.processState = READY;
                
            }else if (metadata.action == "end"){
                cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << "OS: removing process " + processID <<endl;
                
                process.processState = EXIT;
            }
        }
        
        //Process start
        string areaStr;
        string actionStartStr;
        string actionEndStr;
        
        //Instructions of Run
        if (metadata.instructor == "P" && metadata.action == "run") {
            areaStr = "Process " + processID + ":";
            actionStartStr = " start process action";
            actionEndStr = " end process action";
        }
        
        //Instructions of Memory
        
        //New memory allocate function
        if (metadata.instructor == "M") {
            if (metadata.action == "allocate") {
                areaStr = "Process " + processID + ":";
                actionStartStr = " allocating memory";
                stringstream ss;
                ss << std::hex << allocateMemory(config.memoryBolckSize,config.systemMemory);
                string hexStr = ss.str();
                string address = "0x" + hexStr;
                actionEndStr = " memory allocated at " + address ; //TODO
            }
            
            if (metadata.action == "block") {
                areaStr = "Process " + processID + ":";
                actionStartStr = " start memory blocking";
                actionEndStr = " end memory blocking";
            }
        }
        
        //Instructions of I/O
        if (metadata.instructor == "O" || metadata.instructor == "I") {
            
            //Lock I/O operation with mutex
            pthread_mutex_lock(&mutex);
            
            string IOString;
            if (metadata.instructor == "O") {
                IOString = "output";
            }else{
                IOString = "input";
            }
            
            string suffixStr = "";
            
            //Project 3
            if (metadata.action == "printer") {
                int printerNum = allocatePrinter(config.printerCounts);
                suffixStr = " on printer " + to_string(printerNum);
            }
            
            if (metadata.action == "hard drive") {
                int hardDriveNum = allocateHardDrive(config.hardDriveCounts);
                suffixStr = " on HDD " + to_string(hardDriveNum);
            }
            
            areaStr = "Process " + processID + ":";
            actionStartStr = " start " + metadata.action + " " + IOString;
            actionEndStr = " End " + metadata.action + " " + IOString;
            
            process.processState = WAITING;
            double timeIntervalInSeconds = timeOfOperation(config, metadata)/1000.0;
            long timeInMicroSec = (systemRealTime() + timeIntervalInSeconds) * 1000000;
            
            cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << areaStr + actionStartStr + suffixStr <<endl;
            
            pthread_attr_init(&attr);
            pthread_create(&thread, &attr, ThreadProcessing, (void *)(intptr_t)(timeInMicroSec));
            pthread_join(thread, NULL);
            process.processState = RUNNING;
            
            cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << areaStr + actionEndStr <<endl;

            //Unlock the I/O operation with mutex
            pthread_mutex_unlock(&mutex);
        }
        
        if (metadata.instructor == "P" || metadata.instructor =="M") {
            
            process.processState = WAITING;
            
            cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << areaStr + actionStartStr <<endl;
            
            //Let the process running
            double timeIntervalInSeconds = timeOfOperation(config, metadata)/1000.0;
            waitTime(systemRealTime() + timeIntervalInSeconds);
            
            cout << setiosflags(ios::fixed) << timeInterval(flagTime) << " - " << areaStr + actionEndStr <<endl;
            
            process.processState = RUNNING;
        }
        process.metaDataQueue.pop();
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
    
    //Separate long Queue by process and put the processes instance to processQue
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

//Output for Assignment 2
void outputLogSim2(Config config, vector<MetaData> metadatas)
{
    //System start time
    double startTime = systemRealTime();
    //S(Start)0 system start
    cout << setiosflags(ios::fixed) << systemRealTime() - startTime << " - Simulator Program starting" << endl;
    
    /*** application running ***/
    //Processes Queue
    queue<Process> processQue = creatProcessByMetadatas(metadatas);
    
    //Mutiple processes
    while (!processQue.empty()) {
        Process p = processQue.front();
        executeProcess(startTime, config, p/*Process to be executed*/);
        processQue.pop();
    }
    
    //S(end)0 system end
    cout << setiosflags(ios::fixed) << systemRealTime() - startTime << " - Simulator Program ending" << endl;
}

//main function
int main(int argc, const char * argv[]) {
    
    //Get config instance by file path
    Config config = loadConfigData("/Users/xiaolongma/Desktop/OSAssignment/config_1-2.conf");
//    Config config = loadConfigData(argv[1]);

    //Get all metaData
//    vector<MetaData> metadata = loadMetaData(config.filePath);
      vector<MetaData> metadata = loadMetaData("/Users/xiaolongma/Desktop/OSAssignment/test_1a.mdf");

    //Output log
//    outputLog(config,metadata);
    outputLogSim2(config, metadata);
    
    return 0;
}

