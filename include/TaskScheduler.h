#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include "Task.h"

class TaskScheduler {

public:

    TaskScheduler();
    ~TaskScheduler();

    void enqueue(Task* task);
    void startProcessing();
};

#endif /*TASKSCHEDULER_H*/