#include "../include/sysinfo.h"
#include <fstream>
#include <sstream>
#include <string>

CpuSample read_cpu_stat() {
    CpuSample cpu{};
    std::ifstream file("/proc/stat");
    std::string line;
    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream iss(line);
        std::string cpu_label;
        iss >> cpu_label >> cpu.user >> cpu.nice >> cpu.system >> cpu.idle
            >> cpu.iowait >> cpu.irq >> cpu.softirq >> cpu.steal;
    }
    return cpu;
}

MemInfo read_mem_info() {
    MemInfo mem{};
    std::ifstream file("/proc/meminfo");
    std::string key, unit;
    unsigned long value;
    while (file >> key >> value >> unit) {
        if (key == "MemTotal:") mem.mem_total_kb = value;
        else if (key == "MemAvailable:") mem.mem_free_kb = value;
    }
    return mem;
}

