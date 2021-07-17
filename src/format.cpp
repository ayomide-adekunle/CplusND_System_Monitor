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
  
  std::string hr_str, min_str, sec_str = "";
  
  if (hrs < 10){
   hr_str = "0";
  }
  if (min < 10){
   min_str = "0";
  }
  if (sec < 10){
   sec_str = "0";
  }
  
//   if (sec < 10){
//    return std::to_string(hrs) + ":" + std::to_string(min) + ":" +"0"+
//          std::to_string(sec);
//   }
  

  
  return hr_str+std::to_string(hrs) + ":" + min_str+std::to_string(min) + ":" +
         sec_str+std::to_string(sec);
}