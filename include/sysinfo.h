#ifndef SYSINFO_H
#define SYSINFO_H

#include <vector>

struct CpuSample {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
};

struct MemInfo {
    unsigned long mem_total_kb;
    unsigned long mem_free_kb;
};

CpuSample read_cpu_stat();
MemInfo read_mem_info();

#endif

