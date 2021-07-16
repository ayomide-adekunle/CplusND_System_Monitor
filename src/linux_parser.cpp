#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  string os, kernel, version;
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
  // Read and return the system memory utilization
  // ? Calculation : (Total memory - free memory) / Total Memory
  string line, key;
  float sum = 1, free = 1;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "MemTotal:") {
        linestream >> sum;
      } else if (key == "MemAvailable:") {
        linestream >> free;
        break;
      }
    }
  }

  return (sum - free) / sum; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long total_time;
  string line, uptimeStr;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream uptimeStream(line);
    uptimeStream >> uptimeStr;
  }
  total_time = std::stol(uptimeStr);
  return total_time;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
	return LinuxParser::IdleJiffies() + LinuxParser::ActiveJiffies() ;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long time_sum;
  string line, value;
  vector<string> values;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }

  // make sure parsing was correct and values was read
  long utime = 0, stime = 0 , cutime = 0, cstime = 0;
  if (std::all_of(values[13].begin(), values[13].end(), isdigit))
    utime = stol(values[13]);
  if (std::all_of(values[14].begin(), values[14].end(), isdigit))
    stime = stol(values[14]);
  if (std::all_of(values[15].begin(), values[15].end(), isdigit))
    cutime = stol(values[15]);
  if (std::all_of(values[16].begin(), values[16].end(), isdigit))
    cstime = stol(values[16]);

  time_sum = utime + stime + cutime + cstime;
  return time_sum / sysconf(_SC_CLK_TCK);
                                          
  }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  auto all_jiffies = CpuUtilization();

  return stol(all_jiffies[CPUStates::kUser_]) + stol(all_jiffies[CPUStates::kNice_]) +
         stol(all_jiffies[CPUStates::kSystem_]) + stol(all_jiffies[CPUStates::kIRQ_]) +
         stol(all_jiffies[CPUStates::kSoftIRQ_]) +
         stol(all_jiffies[CPUStates::kSteal_]);
  
  //return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto all_jiffies = CpuUtilization();
  return stol(all_jiffies[CPUStates::kIdle_]) + stol(all_jiffies[CPUStates::kIOwait_]); 
  
  //return 0;

}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, cpu, value;
  vector<string> all_jiffies;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    linestream >> cpu;

    while (linestream >> value) {
      all_jiffies.push_back(value);
    }
  }
  return all_jiffies;
  //return {}; }
  
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int all_processes;
  string key, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> all_processes;
        break;
      }
    }
  }
  return all_processes; 

}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int all_processes;
  string key, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> all_processes;
        break;
      }
    }
  }
  return all_processes;
                                    }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 

string given_command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, given_command);
  }
  return given_command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  
  string line, key, memString;
  long mem;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:") {
        linestream >> mem;
        mem /= 1000;
        memString = std::to_string(mem);
        break;
      }
    }
  }
  return memString;
  
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 

string line, key, uid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 

string uid = Uid(pid);
  string id, x, temp, line;
  string username = "DEFAULT";
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      linestream >> temp >> x >> id;
      if (id == uid) {
        username = temp;
        break;
      }
    }
  }
  return username;

}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 

  string line, value;
  vector<string> values;
  long uptime = 0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  try {
    uptime = stol(values[21]) / sysconf(_SC_CLK_TCK);
  } catch (...) {
    uptime = 0;
  }
  return uptime;

}
