#include <utility>
#include <unistd.h>
#include <string.h>
#include <map>
class ProcessStatus {
public:
  static ProcessStatus &GetInstance();
  void PrintMemStatus();
  size_t GetCurrentVmRSS();
  size_t GetMaxVmRSS();
  void ClearMaxVmRSS();
  size_t GetCurrentVmHWM();
  size_t GetMaxVmHWM();
  void ClearMaxVmHWM();

private:
  pid_t pid_;
  std::string path_;
  size_t max_vmrss_ = 0;
  size_t cur_vmrss_ = 0;
  size_t pre_vmrss_ = 0;
  size_t max_vmHWM_ = 0;
  size_t cur_vmHWM_ = 0;
  size_t pre_vmHWM_ = 0;
  size_t max_vmPeak_ = 0;
  size_t cur_vmPeak_ = 0;
  size_t pre_vmPeak_ = 0;

  std::map<std::string, std::string> status_;
  static ProcessStatus *instance_;

  ProcessStatus();
  ProcessStatus(pid_t pid, std::string path);
  ~ProcessStatus();
  ProcessStatus(const ProcessStatus &) = delete;
  ProcessStatus &operator=(const ProcessStatus &) = delete;

  void ProcessStatusUpdate();
  void ProcessVMMAXHWM();
};