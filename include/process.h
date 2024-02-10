// #ifndef PROCESS_H
// #define PROCESS_H

// #include <string>
// /*
// Basic class for Process representation
// It contains relevant attributes as shown below
// */
// class Process {
//  public:
//   int Pid();                               // TODO: See src/process.cpp
//   std::string User();                      // TODO: See src/process.cpp
//   std::string Command();                   // TODO: See src/process.cpp
//   float CpuUtilization();                  // TODO: See src/process.cpp
//   std::string Ram();                       // TODO: See src/process.cpp
//   long int UpTime();                       // TODO: See src/process.cpp
//   bool operator<(Process const& a) const;  // TODO: See src/process.cpp

//   // TODO: Declare any necessary private members
//  private:
// };

// #endif

// #ifndef PROCESS_H
// #define PROCESS_H

// #include <string>

// class Process {
//  public:
//   Process(int pid);

//   int Pid();
//   std::string User();
//   std::string Command();
//   // float CpuUtilization();
//     float CpuUtilization() const; // Add const here

//   std::string Ram();
//   long int UpTime();
//   bool operator<(Process const& a) const;

//  private:
//   int pid_;
// };

// #endif


#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  explicit Process(int pid);  // Constructor with PID

  int Pid() const;  // Return this process's ID
  std::string User() const;  // Return the user (name) that generated this process
  std::string Command() const;  // Return the command that generated this process
  float CpuUtilization() const;  // Return this process's CPU utilization
  std::string Ram() const;  // Return this process's memory utilization
  long int UpTime() const;  // Return the age of this process in seconds
  bool operator<(Process const& a) const;  // Overload the "less than" comparison operator for Process objects

 private:
  int pid_;  // Process ID
};

#endif
