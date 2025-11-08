CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Iinclude
SRC = src/main.cpp src/sysinfo.cpp src/process.cpp
OUT = sysmon

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)

