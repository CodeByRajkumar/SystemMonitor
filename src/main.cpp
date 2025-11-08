#include "../include/sysinfo.h"
#include "../include/process.h"
#include <dirent.h>
#include <iostream>
#include <map>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <iomanip>
#include <fstream>

using ProcMap = std::map<int, unsigned long>;

ProcMap sample_proc_times() {
    ProcMap m;
    DIR *dir = opendir("/proc");
    if (!dir) return m;
    struct dirent *entry;
    Process p;
    while ((entry = readdir(dir)) != nullptr) {
        if (isdigit(entry->d_name[0])) {
            int pid = std::stoi(entry->d_name);
            if (read_process_stat(pid, p)) {
                m[pid] = p.total_time();
            }
        }
    }
    closedir(dir);
    return m;
}

std::vector<Process> build_process_list(
    const ProcMap &prev, const ProcMap &curr,
    const CpuSample &prev_cpu, const CpuSample &curr_cpu)
{
    std::vector<Process> processes;
    double delta_total = (curr_cpu.user + curr_cpu.nice + curr_cpu.system +
                          curr_cpu.idle + curr_cpu.iowait + curr_cpu.irq +
                          curr_cpu.softirq + curr_cpu.steal) -
                         (prev_cpu.user + prev_cpu.nice + prev_cpu.system +
                          prev_cpu.idle + prev_cpu.iowait + prev_cpu.irq +
                          prev_cpu.softirq + prev_cpu.steal);

    long page_kb = sysconf(_SC_PAGE_SIZE) / 1024;
    MemInfo mem = read_mem_info();

    DIR *dir = opendir("/proc");
    if (!dir) return processes;
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (isdigit(entry->d_name[0])) {
            int pid = std::stoi(entry->d_name);
            Process p;
            if (!read_process_stat(pid, p)) continue;

            unsigned long delta_proc = 0;
            auto it_prev = prev.find(pid);
            auto it_curr = curr.find(pid);
            if (it_prev != prev.end() && it_curr != curr.end()) {
                delta_proc = it_curr->second - it_prev->second;
            }

            p.cpu_percent = 100.0 * (static_cast<double>(delta_proc) / delta_total);
            double rss_kb = static_cast<double>(p.rss_pages) * static_cast<double>(page_kb);
            if (mem.mem_total_kb > 0)
                p.mem_percent = 100.0 * (rss_kb / static_cast<double>(mem.mem_total_kb));
            else
                p.mem_percent = 0.0;

            processes.push_back(p);
        }
    }
    closedir(dir);
    return processes;
}

void print_process(const Process &p) {
    long page_kb = sysconf(_SC_PAGE_SIZE) / 1024;
    double rss_kb = static_cast<double>(p.rss_pages) * static_cast<double>(page_kb);
    std::cout << std::setw(6) << p.pid << "  "
              << std::setw(20) << p.name.substr(0, 20) << "  "
              << std::setw(8) << std::fixed << std::setprecision(2) << p.cpu_percent << "  "
              << std::setw(8) << p.mem_percent << "  "
              << std::setw(10) << rss_kb / 1024 << "MB\n";
}

int main() {
    std::cout << "System Monitor Tool (C++)\nPress Ctrl+C to exit.\n";
    while (true) {
        CpuSample prev_cpu = read_cpu_stat();
        ProcMap prev_proc = sample_proc_times();
        sleep(1);
        CpuSample curr_cpu = read_cpu_stat();
        ProcMap curr_proc = sample_proc_times();

        MemInfo mem = read_mem_info();
        double mem_used = (mem.mem_total_kb - mem.mem_free_kb) / 1024.0;

        std::cout << "\033[2J\033[H"; // clear screen
        std::cout << "===== System Information =====\n";
        std::cout << "Memory Used: " << mem_used << " MB / "
                  << mem.mem_total_kb / 1024 << " MB\n";
        std::cout << "==============================\n";
        std::cout << " PID               NAME           CPU(%)    MEM(%)      RSS\n";
        std::cout << "--------------------------------------------------------------\n";

        auto processes = build_process_list(prev_proc, curr_proc, prev_cpu, curr_cpu);
        std::sort(processes.begin(), processes.end(),
                  [](const Process &a, const Process &b) {
                      return a.cpu_percent > b.cpu_percent;
                  });

        for (size_t i = 0; i < std::min<size_t>(10, processes.size()); ++i)
            print_process(processes[i]);

        sleep(2);
    }
}

