#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <vector>
#include <string>
#include "Procces.h"

class Scheduler {
protected:
    std::queue<Process> ready;
    std::queue<Process> blocked;
    std::queue<Process> terminated;
    
    int currentTime;
    int quantom;
    
public:
    Scheduler(int quantom);
    ~Scheduler();

    std::vector<std::string> lines;
    
    void ProcessLines(const std::string& filename);
    int createProcesses(const std::vector<std::string>& lines); 
    
    void runProcess(Process& process);
    void runBlockedProcesses(int elapsedTime);
    
    // Main execution loop
    void run();
};

#endif