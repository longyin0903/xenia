#include "base/logging.h"

namespace base {
namespace logging {
namespace {
class LogVerboseGroup {
 public:
  LogVerboseGroup() { }
  void SetVerboseLevel(int level) { verbose_level_ = level; }
  void Register(int level, const string& module) {
    modules_[module] = level;
  }
  bool ShouldLog(int level, const string& module) const {
    if (level <= 0) { return true; }
    auto it = modules_.find(module);
    if (it == modules_.end()) return level <= verbose_level_;
    return level <= it->second;
  }
 private:
  int verbose_level_ = 0;
  std::map<string, int> modules_;
};
static std::unique_ptr<LogVerboseGroup> kLogVerboseGroup;
}  // namespace

static std::unique_ptr<LogOutputDevice> kLogOutputDevice;

static LogOutputDevice* GetLogOutputDevice() {
  if (kLogOutputDevice == nullptr) {
    kLogOutputDevice.reset(new LogOutputVoidDevice());
  }
  return kLogOutputDevice.get();
}

static LogVerboseGroup* GetLogVerboseGroup() {
  if (kLogVerboseGroup == nullptr) {
    kLogVerboseGroup.reset(new LogVerboseGroup());
  }
  return kLogVerboseGroup.get();
}

void SetLogOutputDevice(LogOutputDevice* device) {
  kLogOutputDevice.reset(device);
}

void SetVLogLevel(int level) {
  GetLogVerboseGroup()->SetVerboseLevel(level);
}

void RegisterVLogModule(int level, const string& module) {
  GetLogVerboseGroup()->Register(level, module);
}

static const char* LogFileNameSuffix(Severity severity) {
  if (severity == INFO) { return ".LOG.INFO"; }
  if (severity == WARNING) { return ".LOG.WARNING"; }
  if (severity == ERROR) { return ".LOG.ERROR"; }
  if (severity == FATAL) { return ".LOG.FATAL"; }
  return ".LOG.UNKNOWN";
}

void LogOutputFileDevice::Send(Severity severity, const string& data) {
  if (data.empty()) { return; }
  if (outputs_[severity] == nullptr) {
    string file_name("/tmp/");
    file_name += app_name_;
    file_name += LogFileNameSuffix(severity);
    outputs_[severity].reset(new std::ofstream(file_name));
  }
  outputs_[severity]->write(data.c_str(), data.size());
}

void LogOutputFileDevice::Reset() {
  outputs_[INFO].reset(nullptr);
  outputs_[WARNING].reset(nullptr);
  outputs_[ERROR].reset(nullptr);
  outputs_[FATAL].reset(nullptr);
}

void LogOutputStringDevice::Send(Severity severity, const string& data) {
  if (output_ != nullptr) { output_->append(data); }
}

void LogOutputStringDevice::Reset() {
  if (output_ != nullptr) { output_->clear(); }
}

static string GetBaseName(const char* file) {
  if (file == nullptr) { return ""; }
  string str(file);
  auto pos = str.rfind('/');
  return pos == string::npos ? str : str.substr(pos + 1);
}

static string GetTidStr() {
  // TODO: get thread id.
  return "tid";
}

LogMessage::LogMessage(const char* file, int line, Severity severity)
    : location_(GetBaseName(file), line), severity_(severity),
      tid_str_(GetTidStr()) {
}

static const char* GetSeverityTag(Severity severity) {
  if (severity == INFO) { return "I"; }
  if (severity == WARNING) { return "W"; }
  if (severity == ERROR) { return "E"; }
  if (severity == FATAL) { return "F"; }
  return "U";
}

LogMessage::~LogMessage() {
  if (stream_.str().empty()) { return; }
  if (!GetLogVerboseGroup()->ShouldLog(verbose_level_, location_.file())) {
    return;
  }
  auto* device = GetLogOutputDevice();
  string str;
  if (print_prefix_) {
    str += GetSeverityTag(severity_);
    str += "<Time>";
    str += " ";
    str += tid_str_;
    str += " ";
    str += location_.ToString();
    str += " ";
  }
  str += stream_.str();
  if (perror_ != 0) {
    str += ": ";
    str += strerror(perror_);
  }
  str += "\n";
  if (output_string_ != nullptr) { *output_string_ = str; }
  device->Send(severity_, str);
  if (severity_ == FATAL) {
    device->Send(INFO, str);
    device->Send(WARNING, str);
    device->Send(ERROR, str);
  } else if (severity_ == ERROR) {
    device->Send(INFO, str);
    device->Send(WARNING, str);
  } else if (severity_ == WARNING) {
    device->Send(INFO, str);
  }
  if (severity_ == FATAL) {
    device->Reset();
    abort();
  }
}

LogMessage& LogMessage::SetPerror() {
  perror_ = errno;
}

ScopedLog::ScopedLog() {
  device_ = std::move(kLogOutputDevice);
  kLogOutputDevice.reset(new LogOutputStringDevice(&log_));
}

ScopedLog::~ScopedLog() {
  Release();
}

void ScopedLog::Release() {
  if (device_ != nullptr) {
    kLogOutputDevice = std::move(device_);
    device_ = nullptr;
  }
}

}  // namsspace logging
}  // namespace base
