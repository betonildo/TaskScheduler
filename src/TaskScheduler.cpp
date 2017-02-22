#include "TaskScheduler.h"

TaskScheduler::TaskScheduler() {
    m_isDone = true;
}

TaskScheduler::~TaskScheduler() {

}

bool TaskScheduler::enqueue(Task* task) {
    std::unique_ptr<Task> taskPtr(task);
    m_taskQueue.push(taskPtr);
    return true;
}

void TaskScheduler::startProcessing() {
    
    m_isDone = false;
    size_t max_threads = std::thread::hardware_concurrency();
    std::vector<std::ref<std::thread>> threads;
    threads.reserve(max_threads);

    while(!m_taskQueue.empty()) {
        std::unique_ptr<Task> task = m_taskQueue.front();
        std::thread t(std::move(*task));
        threads.push_back(std::ref(t));
        m_taskQueue.pop();
    }

    for(auto& tf: threads) {
        tf.join();
    }

    m_isDone = true;
}