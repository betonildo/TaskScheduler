#ifndef TASK_H
#define TASK_H

#include "task_scheduler_local_definitions.h"

class Task {
public:
    enum TaskType{
        Shared,
        Standalone
    };

protected:
    Task::TaskType m_type;    
    std::mutex* m_g_mutex;

public:

    inline Task(TaskType type) {
        m_type = type;
    }

    inline void operator()() {
        run();
    };

    virtual void run() = 0;

    inline std::lock_guard<std::mutex>& getLockGuard() {
        auto lock = new std::lock_guard<std::mutex>(*m_g_mutex);
        return *lock;
    }

    inline void useMutex(std::mutex* g_mutex) {
        m_g_mutex = g_mutex;
    }
};

#endif /*TASK_H*/