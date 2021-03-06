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
#include <list>
#include <unistd.h>

using namespace std;

// Container of the processes for the whole application
queue<Process> appProcessesQue;

//Config info from config files
Config appConfig;

//MetaData from metaData files
vector<MetaData> tasksMetadata;

//This is for generate the Process id number, an increment number
int appProcessIDNumber;

//=======

//Algorithm of SJF
bool sortBySJF(const Process &p1,const Process &p2)
{
    return p1.ExecutionTime < p2.ExecutionTime;
}

//Algorithm of PS
bool sortByPS(const Process &p1,const Process &p2)
{
    return p1.IONumber > p2.IONumber;
}

//Algorithm of SRTF
bool sortBySRTF(const Process &p1,const Process &p2)
{
    return p1.remainingTime < p2.remainingTime;
}

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

//Got the total execution time of a process
int timeOfProcess(Config config,queue<MetaData> metaDataQue)
{
    int timeOfProcess = 0;
    
    while (!metaDataQue.empty()) {
        MetaData metaData = metaDataQue.front();
        timeOfProcess += timeOfOperation(config, metaData);
        metaDataQue.pop();
    }
    
    return timeOfProcess;
}

//Got the total number of IO instructions
int totalIOInstruction(queue<MetaData> metaDataQue)
{
    int totalIOInstructions = 0;
    
    while (!metaDataQue.empty()) {
        MetaData metaData = metaDataQue.front();
        if (metaData.instructor == "O" || metaData.instructor == "I") {
            totalIOInstructions++;
        }
        metaDataQue.pop();
    }
    return totalIOInstructions;
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
//Todo: use semaphore
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
        
        process.executedTime = systemRealTime() - flagTime;
        if (process.executedTime > process.ExecutionTime) {
            process.remainingTime = process.ExecutionTime - process.executedTime;
        }else{
            process.remainingTime = 0;
        }
    }
}

//Get Processes Sequence by metadatas
queue<Process> creatProcessesQueueByMetadatas(Config config,vector<MetaData> metadatas)
{
    //To obtain processes
    queue<Process> processQue;
    
    int processStartNum = 0;
    int processEndNum = 0;
    
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
            nProcess.processID = ++appProcessIDNumber;
            nProcess.processState = START;
            nProcess.metaDataQueue = metaProcessQueue;
            nProcess.ExecutionTime = timeOfProcess(config,metaProcessQueue);
            nProcess.remainingTime = nProcess.ExecutionTime;
            nProcess.IONumber = totalIOInstruction(metaProcessQueue);
            processQue.push(nProcess);
        }
        metaLongQueue.pop();
    }
    return processQue;
}

//======= project 5 ======

void *loadNewProcesses(void * arg)
{
    //Repeat the loading process for 10 times
    for (int i = 0; i < 10; i++) {
        //Every 100ms invoke a creatProcessByMetadatas
        //100ms equals to 100,000 useconds
        usleep(100000);
        
        //every 100ms create a new process queue
        queue<Process> followQue = creatProcessesQueueByMetadatas(appConfig, tasksMetadata);
        
        //Add the new created processes to appProcessQueue
        while (!followQue.empty()) {
            Process p = followQue.front();
            appProcessesQue.push(p);
            followQue.pop();
        }
    }
    pthread_exit(NULL);
}

//Every 100ms load the instructions in metadata file to simulate the running applications
void createApplicationSimilator(Config config,vector<MetaData> metaDatas)
{
    pthread_t pid;
    pthread_create(&pid, NULL, loadNewProcesses, NULL);
//    pthread_join(pid, NULL);
}

//Output for Assignment 3
void outputLogSim3(Config config, vector<MetaData> metadatas)
{
    //System start time
    double startTime = systemRealTime();
    //S(Start)0 system start
    cout << setiosflags(ios::fixed) << systemRealTime() - startTime << " - Simulator Program starting" << endl;
    
    /*** application running ***/
    //Processes Queue
    queue<Process> processQue = creatProcessesQueueByMetadatas(config, metadatas);
    
    queue<Process> executeQue;
    //3. FIFO currently the method
    if (config.cpuSchedulingCode == "FIFO") {
        executeQue = processQue;
    }
    
    //Put into list and use sort method to sort the processes
    list<Process> processV;
    
    //2. PS priority base on numbers of the instructions
    if (config.cpuSchedulingCode == "PS") {
        while (!processQue.empty()) {
            Process p = processQue.front();
            processV.push_back(p);
            processQue.pop();
        }
        processV.sort(sortByPS);
    }
    
    //3. SJF base on remaining time
    if (config.cpuSchedulingCode == "SJF") {
        while (!processQue.empty()) {
            Process p = processQue.front();
            processV.push_back(p);
            processQue.pop();
        }
        processV.sort(sortBySJF);
    }
    
    //Put the sorted processes back to queue
    while (!processV.empty()) {
        Process p = processV.front();
        executeQue.push(p);
        processV.pop_front();
    }
    
    //Mutiple processes
    while (!executeQue.empty()) {
        Process p = executeQue.front();
        executeProcess(startTime, config, p/*Process to be executed*/);
        executeQue.pop();
    }
    
    //S(end)0 system end
    cout << setiosflags(ios::fixed) << systemRealTime() - startTime << " - Simulator Program ending" << endl;
}

void executeProcessWithRR(double flagTime,Process p)
{
    
}

void* QuantumTimerRun(void *t)
{
    double startTime = systemRealTime();
    while (true) {
        usleep(500000);
        
        vector<Process> v;
        while (!appProcessesQue.empty()) {
            v.push_back(appProcessesQue.front());
            appProcessesQue.pop();
        }
        
        int i = 0;
        while (!v.empty()) {
            Process p = v[i];
            executeProcessWithRR(startTime, p);
            if (p.remainingTime <= 0) v.erase(v.begin() + i);
        }
        
    }
    
    pthread_exit(NULL);
}



void RRMEMethod(queue<Process> ProcessQue)
{
    double appStartTime = systemRealTime();
    cout << "This is RR method" << endl;
    
    pthread_attr_init(&attr);
    pthread_create(&thread, &attr, QuantumTimerRun, NULL);
    pthread_join(thread, NULL);
    
    //Each process has 500ms execution time
    //execute a process and then if it is over then pop if it is not over put it back
    
    while (!appProcessesQue.empty()) {
        Process p = appProcessesQue.front();
        appProcessesQue.pop();
    }
}


//Output for assignment 5
void outputLogSim5(Config config, vector<MetaData> metadatas)
{
    //Initial queue
    appProcessesQue = creatProcessesQueueByMetadatas(config, metadatas);

    //follow up tasks every 100ms
    //Load new tasks in every 100ms in this function
    createApplicationSimilator(appConfig, tasksMetadata);

    //Get the App Starting time
    double appStartTime = systemRealTime();

    //Put into list and use sort method to sort the processes
    list<Process> processV;
    
    //Every time sort the queue by our assigned algorithm
    //4. RR Quantum number 50
    if (appConfig.cpuSchedulingCode == "RR") {
        //RR algorithm
        RRMEMethod(appProcessesQue);
    }
    
    //5. SRTF Shortest Time Remaining
    if (appConfig.cpuSchedulingCode == "SRTF") {
        while (!appProcessesQue.empty()) {
            Process p = appProcessesQue.front();
            processV.push_back(p);
            appProcessesQue.pop();
        }
        processV.sort(sortBySRTF);
    }
    
    //Put the sorted processes back to queue
    /*
    while (!processV.empty()) {
        Process p = processV.front();
        appProcessesQue.push(p);
        processV.pop_front();
    }
     */
    
    //Mutiple processes
    //execute the process should be in main thread not this thread
    while (!appProcessesQue.empty()) {
        Process p = appProcessesQue.front();
        executeProcess(appStartTime, appConfig, p);
        appProcessesQue.pop();
    }
}

//main function
int main(int argc, const char * argv[]) {
    
    //Get config instance by file path
    appConfig = loadConfigData("/Users/xiaolongma/Desktop/OSAssignment/config_1-2.conf");
//    Config config = loadConfigData(argv[1]);

    //Get all metaData
//    vector<MetaData> metadata = loadMetaData(config.filePath);
    tasksMetadata = loadMetaData("/Users/xiaolongma/Desktop/OSAssignment/test_1a.mdf");

    //Output log
//    outputLog(config,metadata);
    //outputLogSim3(appConfig, tasksMetadata);
    
    outputLogSim5(appConfig, tasksMetadata);
    
    return 0;
}

