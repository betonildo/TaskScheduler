#ifndef LOCKGUARDER_H
#define LOCKGUARDER_H

#include "task_scheduler_local_definitions.h"

class LockGuarder {

private:

    friend class TaskScheduler;

    std::mutex* m_g_mutex;
    
    inline LockGuarder(std::mutex* gMutex) {
        m_g_mutex = gMutex;
    }

public:
    
    inline std::lock_guard<std::mutex>& create() {
        auto lock = new std::lock_guard<std::mutex>(*m_g_mutex);
        return *lock;
    }

};

#endif /*LOCKGUARDER_H*/