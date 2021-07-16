#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long sec) { 
  int hrs, min;
  hrs = sec / 3600;
  sec = sec % 3600;
  min = sec / 60;
  sec = sec % 60;
  return std::to_string(hrs) + ":" + std::to_string(min) + ":" +
         std::to_string(sec);
}