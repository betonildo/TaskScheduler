#include "TaskScheduler.h"

TaskScheduler::TaskScheduler() {
    m_isDone = true;
}

TaskScheduler::~TaskScheduler() {

}

bool TaskScheduler::enqueue(Task* task) {
    m_taskQueue.push(task);
    return true;
}

void TaskScheduler::startProcessing() {
    
    m_isDone = false;
    size_t max_threads = std::thread::hardware_concurrency();
    std::vector<std::thread*> threads;
    threads.reserve(max_threads);

    while(!m_taskQueue.empty()) {
        Task* task = m_taskQueue.front();
        task->useMutex(&m_mutex);
        std::thread* t = new std::thread(std::ref(*task));
        threads.push_back(t);
        m_taskQueue.pop();
    }

    for(auto tf: threads) {
        tf->join();
    }

    m_isDone = true;
}
