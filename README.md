# System Monitor (C++ - Linux Capstone Project)

## Overview
The **System Monitor** is a lightweight terminal-based tool written in C++ that displays real-time system information, including CPU usage, memory utilization, and active processes on a Linux system.  
It reads data directly from the `/proc` filesystem, similar to the `top` command, to show live system statistics efficiently.

---

## Features
- Displays total and available memory  
- Monitors CPU usage for all processes  
- Shows per-process CPU and memory utilization  
- Auto-refreshes the data periodically  
- Clean, easy-to-read terminal interface  

---


## How It Works
- **process.cpp** → Reads process statistics from `/proc/[pid]/stat`.  
- **sysinfo.cpp** → Extracts CPU and memory details from `/proc/stat` and `/proc/meminfo`.  
- **main.cpp** → Displays and refreshes process data sorted by CPU usage.  

The monitor calculates CPU and memory percentages per process using sampled data between refresh intervals.

---

## Build Instructions
Make sure you are using a Linux environment (Ubuntu recommended).

### 1. Clone the Repository
```bash
git clone https://github.com/CodeByRajkumar/SystemMonitor.git
cd SystemMonitor
```
### 2. Compile the Project
```bash
make
```
### 3. Run the Monitor
```bash
./sysmon
```
### 4. Clean Build Files
```bash
make clean
```
