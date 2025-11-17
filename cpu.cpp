#include <iostream>
#include <vector>
#include <algorithm>

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int startTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
};


void displayTable(std::vector<Process> &processes) {
    std::cout << "PID\tAT\tBT\tST\tCT\tWT\tTAT\n";
    for(size_t i=0;i<processes.size();i++) {
        Process &p = processes[i];
        std::cout << p.pid << "\t" << p.arrivalTime << "\t" << p.burstTime
                  << "\t" << p.startTime << "\t" << p.completionTime
                  << "\t" << p.waitingTime << "\t" << p.turnaroundTime << "\n";
    }
}


void FCFS(std::vector<Process> processes) {
    int currentTime = 0;
    for(size_t i=0;i<processes.size();i++){
        processes[i].startTime = std::max(currentTime, processes[i].arrivalTime);
        processes[i].completionTime = processes[i].startTime + processes[i].burstTime;
        processes[i].waitingTime = processes[i].startTime - processes[i].arrivalTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        currentTime = processes[i].completionTime;
    }

    std::cout << "\n--- FCFS ---\n";
    displayTable(processes);

    
    std::cout << "Gantt Chart: ";
    for(size_t i=0;i<processes.size();i++)
        std::cout << "| P" << processes[i].pid << " ";
    std::cout << "|\n";
}


void SJF(std::vector<Process> processes) {
    int n = processes.size();
    std::vector<bool> done(n,false);
    std::vector<Process> completed;
    int currentTime = 0;

    while(completed.size() < n){
        int idx = -1;
        int minBurst = 1000000; // shumë i madh
        for(int i=0;i<n;i++){
            if(!done[i] && processes[i].arrivalTime <= currentTime && processes[i].burstTime < minBurst){
                idx = i;
                minBurst = processes[i].burstTime;
            }
        }
        if(idx == -1){
            currentTime++;
            continue;
        }
        processes[idx].startTime = currentTime;
        processes[idx].completionTime = currentTime + processes[idx].burstTime;
        processes[idx].waitingTime = processes[idx].startTime - processes[idx].arrivalTime;
        processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
        currentTime = processes[idx].completionTime;
        done[idx] = true;
        completed.push_back(processes[idx]);
    }

    std::cout << "\n--- SJF ---\n";
    displayTable(completed);

    // Gantt Chart
    std::cout << "Gantt Chart: ";
    for(size_t i=0;i<completed.size();i++)
        std::cout << "| P" << completed[i].pid << " ";
    std::cout << "|\n";
}

int main() {
    std::vector<Process> processes;

    Process p1; p1.pid=1; p1.arrivalTime=0; p1.burstTime=5;
    Process p2; p2.pid=2; p2.arrivalTime=1; p2.burstTime=3;
    Process p3; p3.pid=3; p3.arrivalTime=2; p3.burstTime=8;
    Process p4; p4.pid=4; p4.arrivalTime=3; p4.burstTime=6;

    processes.push_back(p1);
    processes.push_back(p2);
    processes.push_back(p3);
    processes.push_back(p4);

    FCFS(processes);
    SJF(processes);

    return 0;
}
