#include "system.h"

#include <unistd.h>

#include <algorithm>  // For std::sort
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// vector<Process>& System::Processes() { return processes_; }

vector<Process>& System::Processes() {
  // Clear the current vector to ensure it contains only current processes
  processes_.clear();

  // Fetch all PIDs
  vector<int> pids = LinuxParser::Pids();

  // Create a Process object for each PID and add it to the processes_ vector
  for (int pid : pids) {
    Process proc(pid);
    processes_.push_back(proc);
  }

  // Optionally: Sort the processes_ vector based on a criterion
  // For example, sort by CPU utilization in descending order
  std::sort(processes_.begin(), processes_.end(),
            [](const Process& a, const Process& b) {
              return a.CpuUtilization() > b.CpuUtilization();
            });

  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
