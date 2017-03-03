#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include "Task.h"
#include "task_scheduler_local_definitions.h"
//#include "fixed_queue.h"

class TaskScheduler {
    std::mutex m_mutex;
    std::queue<Task*> m_taskQueue;
    bool m_isDone;

public:

    TaskScheduler();
    ~TaskScheduler();

    bool enqueue(Task* task);
    void startProcessing();
    bool isDone();
};

#endif /*TASKSCHEDULER_H*/