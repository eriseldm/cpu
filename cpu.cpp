#include <bits/stdc++.h>
using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int start_time = -1;
    int completion = -1;
    int remaining = -1;
};

void print_table(const vector<Process>& procs, const string& title) {
    cout << "\n" << title << "\n";
    cout << "---------------------------------------------------------\n";
    cout << "PID\tArrival\tBurst\tStart\tCompletion\tWaiting\tTurnaround\n";
    cout << "---------------------------------------------------------\n";
    double total_wait = 0, total_turn = 0;
    for (const auto &p : procs) {
        int turnaround = p.completion - p.arrival;
        int waiting = turnaround - p.burst;
        total_wait += waiting;
        total_turn += turnaround;
        cout << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" << p.start_time << "\t" << p.completion << "\t\t" << waiting << "\t" << turnaround << "\n";
    }
    int n = (int)procs.size();
    cout << "---------------------------------------------------------\n";
    cout << "Average waiting time: " << (total_wait / n) << "\n";
    cout << "Average turnaround time: " << (total_turn / n) << "\n";
}

void print_gantt(const vector<tuple<int,int,int>>& segments, const string& title) {
    cout << "\n" << title << " - Gantt Chart (simple ASCII)\n";
    for (auto &seg : segments) {
        int start = get<1>(seg);
        int end = get<2>(seg);
        int len = max(1, end - start);
        cout << "|";
        for (int i=0;i<len;i++) cout << "----";
        cout << ">";
    }
    cout << "\n";
    for (auto &seg : segments) {
        int start = get<1>(seg);
        int end = get<2>(seg);
        int len = max(1, end - start);
        cout << " ";
        int pad = (4*len - 2);
        string label = "P" + to_string(get<0>(seg));
        int left = pad/2 - (int)label.size()/2;
        for (int i=0;i<left;i++) cout << " ";
        cout << label;
        for (int i=0;i<pad-left-(int)label.size();i++) cout << " ";
    }
    cout << "\n";
    for (auto &seg : segments) {
        int start = get<1>(seg);
        int len = max(1, get<2>(seg) - start);
        cout << start;
        for (int i=0;i<len;i++) cout << "    ";
    }
    if (!segments.empty()) cout << get<2>(segments.back());
    cout << "\n";
}

vector<Process> simulate_FCFS(vector<Process> procs, vector<tuple<int,int,int>>& gantt) {
    sort(procs.begin(), procs.end(), [](const Process&a, const Process&b){
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
    });
    int time = 0;
    for (auto &p : procs) p.start_time = -1, p.completion = -1;
    for (auto &p : procs) {
        if (time < p.arrival) time = p.arrival;
        p.start_time = time;
        gantt.push_back({p.pid, time, time + p.burst});
        time += p.burst;
        p.completion = time;
    }
    return procs;
}

vector<Process> simulate_RR(vector<Process> procs, int quantum, vector<tuple<int,int,int>>& gantt) {
    sort(procs.begin(), procs.end(), [](const Process&a, const Process&b){
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
    });
    int n = (int)procs.size();
    for (auto &p : procs) {
        p.remaining = p.burst;
        p.start_time = -1;
        p.completion = -1;
    }
    queue<int> q;
    int time = 0;
    int arrived = 0;
    while (arrived < n && procs[arrived].arrival <= time) { q.push(arrived); arrived++; }
    if (q.empty() && arrived < n) { time = procs[arrived].arrival; q.push(arrived); arrived++; }
    while (!q.empty()) {
        int idx = q.front(); q.pop();
        Process &p = procs[idx];
        if (p.start_time == -1) p.start_time = time;
        int exec = min(quantum, p.remaining);
        gantt.push_back({p.pid, time, time + exec});
        p.remaining -= exec;
        time += exec;
        while (arrived < n && procs[arrived].arrival <= time) { q.push(arrived); arrived++; }
        if (p.remaining > 0) q.push(idx);
        else p.completion = time;
        if (q.empty() && arrived < n) { time = procs[arrived].arrival; q.push(arrived); arrived++; }
    }
    return procs;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << "CPU Scheduling Simulator - FCFS and Round Robin (beginner)\n";
    cout << "------------------------------------------------------\n";
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> procs(n);
    for (int i = 0; i < n; ++i) procs[i].pid = i+1;
    cout << "Enter arrival time and burst time for each process, one per line (e.g. '0 5'):\n";
    for (int i = 0; i < n; ++i) cin >> procs[i].arrival >> procs[i].burst;
    vector<Process> procs_fcfs = procs;
    vector<Process> procs_rr = procs;
    vector<tuple<int,int,int>> gantt_fcfs;
    auto res_fcfs = simulate_FCFS(procs_fcfs, gantt_fcfs);
    print_table(res_fcfs, "Results - FCFS");
    print_gantt(gantt_fcfs, "FCFS");
    int quantum;
    cout << "\nEnter time quantum for Round Robin: ";
    cin >> quantum;
    vector<tuple<int,int,int>> gantt_rr;
    auto res_rr = simulate_RR(procs_rr, quantum, gantt_rr);
    print_table(res_rr, "Results - Round Robin (RR)");
    print_gantt(gantt_rr, "Round Robin (RR)");
    cout << "\nDone. You can modify the input or the code to test more cases.\n";
    return 0;
}   