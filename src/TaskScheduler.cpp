#include "TaskScheduler.h"

TaskScheduler::TaskScheduler() {
    
}

TaskScheduler::~TaskScheduler() {

}

bool TaskScheduler::enqueue(Task* task) {
    m_taskQueue.push(task);
    return true;
}

void TaskScheduler::startProcessing() {

    while(!m_taskQueue.empty()) {
        Task* task = m_taskQueue.front();
        task->run();
        m_taskQueue.pop();
    }
}