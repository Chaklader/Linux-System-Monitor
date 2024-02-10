#include <fstream>
#include <iostream>

#include "ncurses_display.h"
#include "system.h"

std::ofstream log_file("system_monitor.log", std::ios::out | std::ios::app);

void Log(const std::string& message) {
  if (log_file.is_open()) {
    log_file << message << std::endl;
  } else {
    // If the log file isn't open, write the error to standard error.
    std::cerr << "Unable to write to log file: " << message << std::endl;
  }
}

int main() {
  System system;
  NCursesDisplay::Display(system);
}