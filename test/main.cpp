#include "TaskScheduler.h"

#include <iostream>

class PrintTask : public Task {
    std::string m_str;
public:
    PrintTask(std::string str) : Task(Task::TaskType::Standalone){
        m_str = str;
    }
    
    virtual void run() {
        std::cout << m_str << std::endl;
    }
};

class SumTask : public Task {

    TaskScheduler* m_taskScheduler;
    std::atomic_int m_sum;
public:

    SumTask() : Task(Task::TaskType::Standalone) {
        m_sum.store(0);
    }

    virtual void run() {
        m_taskScheduler->enqueue([&](std::mutex* g_mutex) -> void {
            while(true) {
                m_sum++;
            }
        });
    }

    int getPartialResult() {
        return m_sum.load();
    }

    void setTaskScheduler(TaskScheduler& taskScheduler) {
        m_taskScheduler = &taskScheduler;
    }
};


class ReadCmdTask : public Task {

    TaskScheduler* m_taskScheduler;
    SumTask* m_sumtask;
public:

    ReadCmdTask() : Task(Task::TaskType::Standalone) {
        m_sumtask = new SumTask();
    }

    void setTaskScheduler(TaskScheduler& taskScheduler) {
        m_taskScheduler = &taskScheduler;
        m_sumtask->setTaskScheduler(taskScheduler);
    }

    virtual void run() {
        m_taskScheduler->enqueue(m_sumtask);
        bool running = true;
        std::string cmdRead;
        while(running) {

            std::cin >> cmdRead;
            if (cmdRead == "exit") {
                std::cout << "Haaaa" << std::endl;
                running = false;
                m_taskScheduler->exitWhenFinishLastQueue();
            }

            if (cmdRead == "++") {
                int i = 5;
                while(i--)
                    m_taskScheduler->enqueue(new PrintTask("+++++" + i));
            }

            if (cmdRead == "read") {
                std::cout << "Partial: " << m_sumtask->getPartialResult() << std::endl;
            }
        }        
    }
};



int main(int argc, char** argv) {

    PrintTask* pt1 = new PrintTask("Hello");
    PrintTask* pt2 = new PrintTask("My");
    PrintTask* pt3 = new PrintTask("Friend");
    PrintTask* pt4 = new PrintTask("We");
    PrintTask* pt5 = new PrintTask("Meet Again");

    TaskScheduler taskScheduler;

    ReadCmdTask* readerCmd = new ReadCmdTask();
    
    readerCmd->setTaskScheduler(taskScheduler);
    
    taskScheduler.enqueue(readerCmd);
    taskScheduler.enqueue(pt1);
    taskScheduler.enqueue(pt2);
    taskScheduler.enqueue(pt3);
    taskScheduler.enqueue(pt4);
    taskScheduler.enqueue(pt5);

    return taskScheduler.run();
}