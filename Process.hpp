//
//  Process.hpp
//  kk
//
//  Created by Xiaolong Ma on 08/10/2017.
//  Copyright © 2017 TI. All rights reserved.
//

#ifndef Process_hpp
#define Process_hpp

#include <stdio.h>
#include <queue>
#include "MetaData.hpp"

enum ProcessState
{
    START = 0,
    READY,
    RUNNING,
    WAITING,
    EXIT
};

struct PCB {
public:
    int processState;
};

class Process
{
public:
    int processID;
    queue<MetaData> metaDataQueue;
    ProcessState processState;
};

#endif /* Process_hpp */
