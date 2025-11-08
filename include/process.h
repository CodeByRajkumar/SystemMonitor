#ifndef PROCESS_H
#define PROCESS_H

#include <string>

struct Process {
    int pid;
    std::string name;
    unsigned long utime;
    unsigned long stime;
    long rss_pages;
    double cpu_percent;
    double mem_percent;

    unsigned long total_time() const {
        return utime + stime;
    }
};

// Reads process stats from /proc/[pid]/stat
bool read_process_stat(int pid, Process &proc);

#endif

