#ifndef TASK_H
#define TASK_H

#include "task_scheduler_local_definitions.h"
#define SCOPELOCK std::lock_guard<std::mutex> lock##__COUNTER__(*m_g_mutex);
#define SCOPEGUARDIAN(INNERSCOPE) ([&]() { \
    {   \
        SCOPELOCK \
        {INNERSCOPE} \
    }   \
})()

class Task {

protected:
    std::mutex* m_g_mutex;

public:

    inline Task() {}

    virtual void process(std::function<void()> whenDone) = 0;

    inline void useMutex(std::mutex* g_mutex) {
        m_g_mutex = g_mutex;
    }
};

#endif /*TASK_H*/