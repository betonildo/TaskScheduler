#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include "Task.h"
#include "task_scheduler_local_definitions.h"
//#include "fixed_queue.h"

class TaskScheduler {

    std::queue<Task*> m_taskQueue;

public:

    TaskScheduler();
    ~TaskScheduler();

    bool enqueue(Task* task);
    void startProcessing();
};

#endif /*TASKSCHEDULER_H*/