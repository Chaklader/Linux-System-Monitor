#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <limits>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  long total_mem = 0;
  long free_mem = 0;
  long buffers_mem = 0;
  long cached_mem = 0;

  std::string token;
  std::ifstream file("/proc/meminfo");
  while (file >> token) {
    string str_temp;
    if (token == "MemTotal:") {
      file >> str_temp;
      total_mem = std::stol(str_temp);
    } else if (token == "MemFree:") {
      file >> str_temp;
      free_mem = std::stol(str_temp);
    } else if (token == "Buffers:") {
      file >> str_temp;
      buffers_mem = std::stol(str_temp);
    } else if (token == "Cached:") {
      file >> str_temp;
      cached_mem = std::stol(str_temp);
    }

    // Ignore the rest of the line
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  long use_mem = free_mem + buffers_mem + cached_mem;
  float memory_utilization = (total_mem - use_mem) / float(total_mem);

  return memory_utilization;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long activeJiffies = 0;

  string line;
  string value;
  vector<string> statValues;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      statValues.push_back(value);
    }
  }

  // Calculate active jiffies for the process
  // Sum utime (14), stime (15), cutime (16) and cstime (17)
  if (statValues.size() >= 18) {
    for (int i = 14; i <= 17; i++) {
      activeJiffies += stol(statValues[i]);
    }
  }

  return activeJiffies;
}

// TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }
long LinuxParser::ActiveJiffies() {
  vector<string> cpuUtilization = CpuUtilization();

  long activeJiffies = 0;

  activeJiffies += stol(cpuUtilization[kUser_]);
  activeJiffies += stol(cpuUtilization[kNice_]);
  activeJiffies += stol(cpuUtilization[kSystem_]);
  activeJiffies += stol(cpuUtilization[kIRQ_]);
  activeJiffies += stol(cpuUtilization[kSoftIRQ_]);
  activeJiffies += stol(cpuUtilization[kSteal_]);

  return activeJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpuUtilization = CpuUtilization();
  // Idle jiffies for the system: idle + iowait
  long idleJiffies =
      stol(cpuUtilization[kIdle_]) + stol(cpuUtilization[kIOwait_]);
  return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, key;
  vector<string> cpuUtilization;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;  // Skip "cpu" identifier
    while (linestream >> key) {
      cpuUtilization.push_back(key);
    }
  }
  return cpuUtilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return value;
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return value;
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return string();
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value;
  long ramKB;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        ramKB = stol(value);
        return to_string(ramKB / 1024);  // Convert from KB to MB
      }
    }
  }
  return string();
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line, user, x, uidFromFile;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uidFromFile;
      if (uidFromFile == uid) {
        return user;
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  string value;
  vector<string> statValues;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    while (linestream >> value) {
      statValues.push_back(value);
    }
  }

  // uptime = system uptime (UpTime()) - process start time since boot (22 /
  // sysconf(_SC_CLK_TCK))
  long uptime = UpTime() - stol(statValues[22]) / sysconf(_SC_CLK_TCK);

  return uptime;
}
