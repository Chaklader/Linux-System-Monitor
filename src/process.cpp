// #include "process.h"

// #include <unistd.h>

// #include "linux_parser.h"

// // TODO: Return this process's ID
// int Process::Pid() { return pid_; }

// // TODO: Return this process's CPU utilization
// float Process::CpuUtilization() const {
//   return LinuxParser::ActiveJiffies(pid_) /
//          static_cast<float>(sysconf(_SC_CLK_TCK));
// }

// // TODO: Return the command that generated this process
// std::string Process::Command() { return LinuxParser::Command(pid_); }

// // TODO: Return this process's memory utilization
// std::string Process::Ram() { return LinuxParser::Ram(pid_); }

// // TODO: Return the user (name) that generated this process
// std::string Process::User() { return LinuxParser::User(pid_); }

// // TODO: Return the age of this process in seconds
// long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// // TODO: Overload the "less than" comparison operator for Process objects
// bool Process::operator<(Process const& a) const { return this->pid_ < a.pid_; }


#include "process.h"
#include "linux_parser.h"
#include <unistd.h>

Process::Process(int pid) : pid_(pid) {}

int Process::Pid() const { return pid_; }

std::string Process::User() const {
  return LinuxParser::User(pid_);
}

std::string Process::Command() const {
  return LinuxParser::Command(pid_);
}

float Process::CpuUtilization() const {
  long activeTime = LinuxParser::ActiveJiffies(pid_);
  long startTime = LinuxParser::UpTime(pid_);
  long systemUptime = LinuxParser::UpTime();
  long uptime = systemUptime - (startTime / sysconf(_SC_CLK_TCK));

  if (uptime == 0) return 0.0;  // Avoid division by zero
  return static_cast<float>(activeTime) / sysconf(_SC_CLK_TCK) / uptime;
}

std::string Process::Ram() const {
  return LinuxParser::Ram(pid_);
}

long int Process::UpTime() const {
  return LinuxParser::UpTime(pid_);
}

bool Process::operator<(Process const& a) const {
  // Example comparison based on CPU utilization; adjust as needed
  return this->CpuUtilization() < a.CpuUtilization();
}
