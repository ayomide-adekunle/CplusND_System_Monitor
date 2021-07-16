#include "processor.h"
#include "linux_parser.h"
#include <iostream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  
  // Return the aggregate CPU utilization
 long total_old, total_new, active_new, idle_old, idle_new;
  total_new = LinuxParser::Jiffies();
  active_new =  LinuxParser::ActiveJiffies();
  idle_new = LinuxParser::IdleJiffies();

  total_old = total_;
  idle_old = idle_;

  Update(idle_new, active_new, total_new);

  float totalDelta = float(total_new) - float(total_old);
  float idleDetla = float(idle_new) - float(idle_old);

  float utilization = (totalDelta - idleDetla) / totalDelta;
  return utilization;
  
 }


void Processor::Update(long idle, long active, long total) {
  idle_ = idle;
  active_ = active;
  total_ = total;
}