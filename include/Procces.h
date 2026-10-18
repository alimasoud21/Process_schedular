#ifndef PROCESS_H
#define PROCESS_H

#include <queue>
#include <vector>
#include <string>

enum class ProcessState { NEW, READY, RUNNING, WAITING, TERMINATED };

struct Burst{
    std::string type; // cpu or io
    int duration; // in micro seconds
};

class Process {
private:
    int pid;
    int arrivalTime;
    ProcessState state;
    std::vector<Burst> bursts;

public:
    Process(int pid, int arrivalTime, const std::vector<Burst> bursts);
    
    // State management
    int getPid() const;
    int getArrivalTime() const; // Fixed typo
    ProcessState getState() const;
    void setState(ProcessState newState);

    // Added to allow scheduler to view/modify the bursts
    std::vector<Burst>& getBursts(); 

    static std::vector<Burst> textParser(const std::string line);

    int procesing(int burst_duration);
};

#endif