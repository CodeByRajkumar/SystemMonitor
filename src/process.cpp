#include "../include/process.h"
#include <fstream>
#include <sstream>
#include <unistd.h>

bool read_process_stat(int pid, Process &proc) {
    std::string path = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream file(path);
    if (!file.is_open()) return false;

    proc.pid = pid;
    std::string comm, state;
    unsigned long dummy;
    long rss;
    file >> proc.pid >> comm >> state;

    // skip to utime (14th) and stime (15th) fields
    for (int i = 0; i < 11; ++i) file >> dummy;
    file >> proc.utime >> proc.stime;

    // skip to rss (24th field)
    for (int i = 0; i < 7; ++i) file >> dummy;
    file >> rss;
    proc.rss_pages = rss;

    proc.name = comm.substr(1, comm.size() - 2); // remove parentheses
    return true;
}

