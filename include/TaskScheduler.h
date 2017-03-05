#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include "Task.h"
#include "task_scheduler_local_definitions.h"
//#include "fixed_queue.h"

class TaskScheduler {

private:
    std::mutex m_mutex;
    std::queue<Task*> m_taskQueue;
    std::queue<std::function<void()> > m_functionQueue;
    std::queue<std::thread*> m_execQueue;
    std::atomic_bool m_isDone;
    std::atomic_uint m_maxThreads;

    std::thread* m_CreateThreadWith(Task* task);

public:

    TaskScheduler();
    ~TaskScheduler();

    void enqueue(Task* task);
    void enqueue(std::function<void(std::mutex*)> f);
    int run();
    bool isDone();
    void exitWhenFinishLastQueue();
};

#endif /*TASKSCHEDULER_H*/