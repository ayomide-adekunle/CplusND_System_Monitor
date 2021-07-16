#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//defines a constructor for the class
Process::Process(int pid) {
  pid_ = pid;
  command_ = LinuxParser::Command(pid);
  std::string ram = LinuxParser::Ram(pid);
  ram_ = std::stol(ram);
  uptime_ = LinuxParser::UpTime(pid);
  user_ = LinuxParser::User(pid);

  long sec = LinuxParser::UpTime() - uptime_;
  long total_sec = LinuxParser::ActiveJiffies(pid);
  try {
    utilization_ = float(total_sec) / float(sec);

  } catch (...) {
    utilization_ = 0;
  }
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return utilization_; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return std::to_string(ram_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const {
  return CpuUtilization() < a.CpuUtilization();
}