#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int remainingSeconds = seconds % 60;

  return std::to_string(hours) + ":" + (minutes < 10 ? "0" : "") +
         std::to_string(minutes) + ":" + (remainingSeconds < 10 ? "0" : "") +
         std::to_string(remainingSeconds);
}
