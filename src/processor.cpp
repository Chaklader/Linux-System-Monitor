#include "processor.h"

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  // Get the active and idle jiffies
  long activeJiffies = LinuxParser::ActiveJiffies();
  long idleJiffies = LinuxParser::IdleJiffies();

  // Calculate the total jiffies
  long totalJiffies = activeJiffies + idleJiffies;

  // Calculate the CPU utilization as the ratio of active jiffies to total
  // jiffies
  float utilization = static_cast<float>(activeJiffies) / totalJiffies;

  return utilization;
}