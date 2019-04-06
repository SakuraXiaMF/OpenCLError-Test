#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>
#include <process.h>

ProcessStatus *ProcessStatus::instance_ = nullptr;

ProcessStatus &ProcessStatus::GetInstance() {
  if (instance_ == nullptr) {
    instance_ = new ProcessStatus;
  }
  return *instance_;
}

void ProcessStatus::PrintMemStatus() {
  ProcessStatusUpdate();
  if (cur_vmrss_ == pre_vmrss_)
    return;
  // LOG(INFO)<<"VmRSS = "<<status_["VmRSS"]<<" max_VmRSS = "<<max_vmrss_<<"
  // kB";
}

size_t ProcessStatus::GetCurrentVmRSS() {
  ProcessStatusUpdate();
  const std::string &s = status_["VmRSS"];
  return (size_t)atoi(s.substr(0, s.length() - 3).c_str());
}

size_t ProcessStatus::GetCurrentVmHWM() {
  ProcessStatusUpdate();
  const std::string &s = status_["VmHWM"];
  return (size_t)atoi(s.substr(0, s.length() - 3).c_str());
}

size_t ProcessStatus::GetMaxVmRSS() {
  ProcessStatusUpdate();
  return max_vmrss_;
}

size_t ProcessStatus::GetMaxVmHWM() {
  ProcessVMMAXHWM();
  return max_vmHWM_;
}

void ProcessStatus::ClearMaxVmRSS() { max_vmrss_ = 0; }
void ProcessStatus::ClearMaxVmHWM() { max_vmHWM_ = 0; }

ProcessStatus::ProcessStatus() : pid_(getpid()) {
  char path[64] = {};
  sprintf(path, "/proc/%d/status", (int)pid_);
  path_ = path;
  //LOG(INFO) << "new process status.";
}

ProcessStatus::ProcessStatus(pid_t pid, std::string path)
    : pid_(pid), path_(path) {}

void ProcessStatus::ProcessStatusUpdate() {//VMRSS
  pre_vmrss_ = cur_vmrss_;
  std::ifstream in;
  in.open(path_.c_str(), std::ifstream::in);
  std::string line;
  while (!in.eof()) {
    std::getline(in, line);
    size_t t = line.find_first_of(':');
    if (t <= line.length() - 2) {
      std::string cont(line.substr(t + 2, line.length() - t - 2));
      cont.assign(cont.begin() + cont.find_first_not_of(' '),
                  cont.begin() + cont.find_last_not_of(' ') + 1);
      status_[line.substr(0, t)] = cont;
    }
  }
  in.close();
  const std::string &s = status_["VmRSS"];
  cur_vmrss_ = (size_t)atoi(s.substr(0, s.length() - 3).c_str());
  max_vmrss_ = std::max(max_vmrss_, cur_vmrss_);
} 

void ProcessStatus::ProcessVMMAXHWM() { // VMHWM
  pre_vmHWM_ = cur_vmHWM_;
  std::ifstream in;
  in.open(path_.c_str(), std::ifstream::in);
  std::string line;
  while (!in.eof()) {
    std::getline(in, line);
    size_t t = line.find_first_of(':');
    if (t <= line.length() - 2) {
      std::string cont(line.substr(t + 2, line.length() - t - 2));
      cont.assign(cont.begin() + cont.find_first_not_of(' '),
                  cont.begin() + cont.find_last_not_of(' ') + 1);
      status_[line.substr(0, t)] = cont;
    }
  }const std::string &s = status_["VmHWM"];
	cur_vmHWM_ = (size_t)atoi(s.substr(0, s.length() - 3).c_str());
	max_vmHWM_ = std::max(max_vmrss_, cur_vmrss_);
  in.close();
} // namespace tensorflow