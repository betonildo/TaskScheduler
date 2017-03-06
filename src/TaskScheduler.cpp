#include "TaskScheduler.h"

TaskScheduler::TaskScheduler() {
    m_isDone.store(false);
    m_maxThreads.store(std::thread::hardware_concurrency());
}

TaskScheduler::~TaskScheduler() {

}

void TaskScheduler::exitWhenFinishLastQueue() {
    m_isDone.store(true);
}

void TaskScheduler::enqueue(Task* task) {
    
    task->useMutex(&m_mutex);
    if (m_maxThreads.load() > 0) m_execQueue.push(m_CreateThreadWith(task));
    else m_taskQueue.push(task);
}

void TaskScheduler::enqueue(std::function<void(std::function<void()> )> f) {
    //std::bind(f, &m_mutex, whenDone);   

    std::function<void()> bound_f = [=] {
        this->m_maxThreads--;
        std::function<void()> whenDone = [=] { this->m_maxThreads++; };
        f(whenDone);
    };

    if (m_maxThreads.load() > 0) m_execQueue.push(m_CreateThreadWith(bound_f));
    else m_functionQueue.push(bound_f);
}

int TaskScheduler::run() {

    // handle
    while(!m_isDone.load()) {
        if (m_maxThreads.load() > 0) {
            if(!m_taskQueue.empty()) {
                auto task = m_taskQueue.front();
                m_execQueue.push(m_CreateThreadWith(task));
                m_taskQueue.pop();
            }

            if (!m_functionQueue.empty()) {
                auto f = m_functionQueue.front();
                m_execQueue.push(m_CreateThreadWith(f));
                m_functionQueue.pop();
            }

            // automagically finish processing when all are done
            if (m_maxThreads.load() == std::thread::hardware_concurrency() && m_taskQueue.empty() && m_functionQueue.empty()) {
                exitWhenFinishLastQueue();
            }
        }
    }

    while (!m_execQueue.empty()) {
        auto t = m_execQueue.front();
        t->join();
        m_execQueue.pop();
    }

    return 0;
}

std::thread* TaskScheduler::m_CreateThreadWith(Task* task) {
    
    this->m_maxThreads--;
    return new std::thread([=] {
        
        // when done increase max available
        task->process([=] {
            this->m_maxThreads++;
        });
    });
}

std::thread* TaskScheduler::m_CreateThreadWith(std::function<void()> f) {
    return new std::thread(f);
}