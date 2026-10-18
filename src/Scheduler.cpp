#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <algorithm>

Scheduler::Scheduler(int quantom) : currentTime(0), quantom(quantom) {}

Scheduler::~Scheduler() {}

void Scheduler::ProcessLines(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    std::cout << "[Scheduler] Loaded " << lines.size() << " processes from file.\n";
}

int Scheduler::createProcesses(const std::vector<std::string>& parsedLines) {
    int pid = 1;
    for (const auto& line : parsedLines) {
        std::vector<Burst> parsedBursts = Process::textParser(line);
        Process p(pid++, currentTime, parsedBursts);
        p.setState(ProcessState::READY);
        ready.push(p);
    }
    return pid - 1;
}

void Scheduler::runBlockedProcesses(int elapsedTime) {
    int size = blocked.size();
    for (int i = 0; i < size; ++i) {
        Process p = blocked.front();
        blocked.pop();

        if (!p.getBursts().empty()) {
            // Reduce IO burst by the elapsed CPU time
            p.getBursts().front().duration -= elapsedTime;
            
            if (p.getBursts().front().duration <= 0) {
                // IO finished, remove the IO burst and move to ready queue
                p.getBursts().erase(p.getBursts().begin());
                if (!p.getBursts().empty()) {
                    p.setState(ProcessState::READY);
                    ready.push(p);
                    std::cout << "Time " << currentTime << ": Process " << p.getPid() << " completed I/O. Moving to READY.\n";
                } else {
                    p.setState(ProcessState::TERMINATED);
                    terminated.push(p);
                    std::cout << "Time " << currentTime << ": Process " << p.getPid() << " TERMINATED.\n";
                }
            } else {
                // IO not finished, put back in blocked
                blocked.push(p);
            }
        }
    }
}

void Scheduler::runProcess(Process& process) {
    process.setState(ProcessState::RUNNING);
    
    if (process.getBursts().empty()) return;

    Burst& currentBurst = process.getBursts().front();
    
    // Determine how much time this process will actually run
    int timeToRun = std::min(quantom, currentBurst.duration);
    
    // Simulate processing
    process.procesing(timeToRun);
    
    currentTime += timeToRun;
    currentBurst.duration -= timeToRun;

    std::cout << "Time " << currentTime << ": Process " << process.getPid() 
              << " ran on CPU for " << timeToRun << "us. (Remaining CPU burst: " 
              << currentBurst.duration << "us)\n";

    // Simultaneously process blocked queue I/O while CPU was running
    runBlockedProcesses(timeToRun);

    if (currentBurst.duration == 0) {
        // CPU burst ended
        process.getBursts().erase(process.getBursts().begin());
        
        if (process.getBursts().empty()) {
            process.setState(ProcessState::TERMINATED);
            terminated.push(process);
            std::cout << "Time " << currentTime << ": Process " << process.getPid() << " TERMINATED.\n";
        } else {
            // Next burst is an I/O burst
            process.setState(ProcessState::WAITING);
            blocked.push(process);
            std::cout << "Time " << currentTime << ": Process " << process.getPid() << " moving to BLOCKED for I/O.\n";
        }
    } else {
        // CPU burst didn't end, goes back to ready queue (Round Robin)
        process.setState(ProcessState::READY);
        ready.push(process);
    }
}

void Scheduler::run() {
    std::cout << "\n--- Starting Round Robin Scheduler (Quantum = " << quantom << ") ---\n";
    while (!ready.empty() || !blocked.empty()) {
        if (!ready.empty()) {
            Process currentProcess = ready.front();
            ready.pop();
            runProcess(currentProcess);
        } else {
            // CPU is idle, fast-forward time to next I/O completion
            currentTime += 1;
            runBlockedProcesses(1);
        }
    }
    std::cout << "\n--- All Processes Terminated. Simulation ended at Time " << currentTime << " ---\n";
}

int main() {
    Scheduler scheduler(10);
    
    scheduler.ProcessLines("file.txt");
    scheduler.createProcesses(scheduler.lines);
    
    scheduler.run();
    
    return 0;
}