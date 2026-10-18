#include "Procces.h"
#include <iostream>
#include <sstream>

Process::Process(int pid, int arrivalTime, const std::vector<Burst> bursts)
    : pid(pid), arrivalTime(arrivalTime), bursts(bursts), state(ProcessState::NEW) {}

int Process::getPid() const { return pid; }
int Process::getArrivalTime() const { return arrivalTime; }
ProcessState Process::getState() const { return state; }

void Process::setState(ProcessState newState) {
    state = newState;
}

std::vector<Burst>& Process::getBursts() {
    return bursts;
}

std::vector<Burst> Process::textParser(const std::string line) {
    std::vector<Burst> result;
    std::stringstream ss(line);
    int num;
    int i = 1;

    while (ss >> num){
        Burst burst;
        burst.type = ((i % 2) != 0 ) ? "cpu" : "io";
        burst.duration = num;
        result.push_back(burst);
        i++;
    }
    return result;
}

// Fixed logic to just simulate passing time and return remainder
int Process::procesing(int burst_duration) {
    while(burst_duration > 0){
        burst_duration -= 1;
    }
    return burst_duration; 
}