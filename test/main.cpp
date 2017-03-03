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

int main(int argc, char** argv) {

    PrintTask* pt1 = new PrintTask("Hello");
    PrintTask* pt2 = new PrintTask("My");
    PrintTask* pt3 = new PrintTask("Friend");
    PrintTask* pt4 = new PrintTask("We");
    PrintTask* pt5 = new PrintTask("Meet Again");

    TaskScheduler taskScheduler;

    taskScheduler.enqueue(pt1);
    taskScheduler.enqueue(pt2);
    taskScheduler.enqueue(pt3);
    taskScheduler.enqueue(pt4);
    taskScheduler.enqueue(pt5);


    taskScheduler.startProcessing();

    return 0;
}