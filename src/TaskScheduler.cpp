#include "TaskScheduler.h"

TaskScheduler::TaskScheduler() {
    m_isDone.store(false);
    m_maxThreads.store(std::thread::hardware_concurrency());
    std::cout << "Max threads: " << m_maxThreads.load() << std::endl;
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

void TaskScheduler::enqueue(std::function<void(std::mutex*)> f) {
    
    std::function<void()> bound_f = std::bind(f, &m_mutex);
    if (m_maxThreads.load() > 0) m_execQueue.push(new std::thread(bound_f));
    else m_functionQueue.push(bound_f);
}

std::thread* TaskScheduler::m_CreateThreadWith(Task* task) {
    this->m_maxThreads--;
    return new std::thread([=] {
        this->m_maxThreads++;
        task->run();    
    });
}

int TaskScheduler::run() {

    // handle
    while(!m_isDone.load()) {
        if (m_maxThreads.load() > 0 && !m_taskQueue.empty()) {
            auto task = m_taskQueue.front();
            m_execQueue.push(m_CreateThreadWith(task));
            m_taskQueue.pop();
        }

        if (m_maxThreads.load() > 0 && !m_functionQueue.empty()) {
            auto f = m_functionQueue.front();
            m_execQueue.push(new std::thread(f));
            m_functionQueue.pop();
        }
    }

    while (!m_execQueue.empty()) {
        auto t = m_execQueue.front();
        t->join();
        m_execQueue.pop();
    }

    return 0;
}
