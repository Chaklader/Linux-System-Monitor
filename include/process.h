#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  explicit Process(int pid);  // Constructor with PID

  int Pid() const;  // Return this process's ID
  std::string User()
      const;  // Return the user (name) that generated this process
  std::string Command()
      const;  // Return the command that generated this process
  float CpuUtilization() const;  // Return this process's CPU utilization
  std::string Ram() const;       // Return this process's memory utilization
  long int UpTime() const;       // Return the age of this process in seconds
  bool operator<(
      Process const& a) const;  // Overload the "less than" comparison operator
                                // for Process objects

 private:
  int pid_;  // Process ID
};

#endif
