#include "TaskScheduler.h"

#include <iostream>
#include <string>

class PrintTask : public Task {
    std::string m_str;
public:
    PrintTask(std::string str) {
        m_str = str;
    }
    
    virtual void process(std::function<void()> whenDone) {
        SCOPEGUARDIAN({
            std::cout << m_str << std::endl;
            whenDone();
        });
    }
};

class SumTask : public Task {

    TaskScheduler* m_taskScheduler;
    std::atomic_int m_sum;
    bool m_exit;
public:

    SumTask() {
        m_sum.store(0);
        m_exit = false;
    }

    virtual void process(std::function<void()> whenDone) {
        
        m_taskScheduler->enqueue([&](std::function<void()> whenDoneFunction) -> void {
            while(!m_exit) m_sum++;
            whenDoneFunction();
        });

        whenDone();
    }

    int getPartialResult() {
        return m_sum.load();
    }

    void exit() {
        m_exit = true;
    }

    void setTaskScheduler(TaskScheduler& taskScheduler) {
        m_taskScheduler = &taskScheduler;
    }
};


class ReadCmdTask : public Task {

    TaskScheduler* m_taskScheduler;
    SumTask* m_sumtask;
public:

    ReadCmdTask() {
        m_sumtask = new SumTask();
    }

    void setTaskScheduler(TaskScheduler& taskScheduler) {
        m_taskScheduler = &taskScheduler;
        m_sumtask->setTaskScheduler(taskScheduler);
    }

    virtual void process(std::function<void()> whenDone) {
        m_taskScheduler->enqueue(m_sumtask);
        bool running = true;
        std::string cmdRead;
        while(running) {
            
            std::cin >> cmdRead;
            SCOPEGUARDIAN({
                if (cmdRead == "exit") {
                    m_sumtask->exit();
                    std::cout << "Haaaa" << std::endl;
                    running = false;
                    m_taskScheduler->exitWhenFinishLastQueue();
                }

                if (cmdRead == "++") {
                    int i = 5;
                    while(i--)
                        m_taskScheduler->enqueue(new PrintTask("+++++" + std::to_string(i)));
                }

                if (cmdRead == "read") {
                    std::cout << "Partial: " << m_sumtask->getPartialResult() << std::endl;
                }
            });
        }

        whenDone();
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

    taskScheduler.enqueue([](std::function<void()> whenDone) {
        std::cout << "LLLLLLLLLLLLLLLLLLLLLLLLL" << std::endl;
        whenDone();
    });
    taskScheduler.enqueue(pt1);
    taskScheduler.enqueue(pt2);
    taskScheduler.enqueue(pt3);
    taskScheduler.enqueue(pt4);
    taskScheduler.enqueue(pt5);

    return taskScheduler.run();
}