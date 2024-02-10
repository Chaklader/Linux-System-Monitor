#include "process.h"

#include "linux_parser.h"
#include <unistd.h>


// TODO: Return this process's ID
int Process::Pid() {
    return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    return LinuxParser::ActiveJiffies(pid_) / static_cast<float>(sysconf(_SC_CLK_TCK));
}

// TODO: Return the command that generated this process 
std::string Process::Command() {
  return LinuxParser::Command(pid_);  
}

// TODO: Return this process's memory utilization
std::string Process::Ram() {
  return LinuxParser::Ram(pid_); 
}

// TODO: Return the user (name) that generated this process
std::string Process::User() {
    return LinuxParser::User(pid_); 
}

// TODO: Return the age of this process in seconds
long int Process::UpTime() {
    return LinuxParser::UpTime(pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return this->pid_ < a.pid_; 
}