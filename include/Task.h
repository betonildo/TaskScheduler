#ifndef TASK_H
#define TASK_H

#include <thread>
#include <mutex>

class Task {
public:
    enum TaskType{
        Shared,
        Single
    };

private:
    Task::TaskType m_type;    
    std::mutex* m_g_mutex;
public:



    Task(TaskType type) {
        m_type = type;
    }  

    virtual void run() = 0;

    void operator()() {
        if (m_type == Shared) {
            std::lock_guard<std::mutex> lock(*m_g_mutex);
            run();
        }
        else {
            run();
        }
    };

    void useMutex(std::mutex* g_mutex) {
        m_g_mutex = g_mutex;
    }
};

#endif /*TASK_H*/