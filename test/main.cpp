#include "TaskScheduler.h"

#include <iostream>
#include <mutex>


int pn = 0;

class PrintTask : public Task {



public:

    virtual void operator()() {
        std::cout << "pn: " << pn++ << std::endl;
    }

};

int main(int argc, char** argv) {

    PrintTask* pt1 = new PrintTask();
    PrintTask* pt2 = new PrintTask();
    PrintTask* pt3 = new PrintTask();
    PrintTask* pt4 = new PrintTask();
    PrintTask* pt5 = new PrintTask();

    TaskScheduler taskScheduler;

    taskScheduler.enqueue(pt1);
    taskScheduler.enqueue(pt2);
    taskScheduler.enqueue(pt3);
    taskScheduler.enqueue(pt4);
    taskScheduler.enqueue(pt5);


    taskScheduler.startProcessing();

    return 0;
}