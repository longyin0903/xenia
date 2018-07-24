#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include "base/file_location.h"

namespace base {

namespace logging {

// The severity of logging message. Do not change order of enumerations.
enum Severity {
  INFO,
  WARNING,
  ERROR,
  FATAL
};

// The abstract device of log output.
class LogOutputDevice {
 public:
  virtual ~LogOutputDevice() { }
  virtual void Send(Severity severity, const string& msg) = 0;
  virtual void Reset() = 0;
};

// The device for log output into file.
class LogOutputFileDevice : public LogOutputDevice {
 public:
  explicit LogOutputFileDevice(string app_name)
      : app_name_(std::move(app_name)) { }
  void Send(Severity severity, const string& msg) override;
  void Reset() override;
 private:
  const string app_name_;
  std::map<Severity, std::unique_ptr<std::ofstream>> outputs_;
};

// The device for log output into string
class LogOutputStringDevice : public LogOutputDevice {
 public:
  explicit LogOutputStringDevice(string *output) : output_(output) { }
  void Send(Severity severity, const string& msg) override;
  void Reset() override;
 private:
  string *const output_;  
};

class LogOutputVoidDevice : public LogOutputDevice {
 public:
  LogOutputVoidDevice() { }
  ~LogOutputVoidDevice() override { }
  void Send(Severity, const string&) override { }
  void Reset() override { }
};

// Set and take the ownership of device.
void SetLogOutputDevice(LogOutputDevice* device);
void SetVLogLevel(int level);
void RegisterVLogModule(int level, const string& module);

struct NoPrefixTag { };
inline NoPrefixTag no_prefix() { return NoPrefixTag(); }

class LogMessage {
 public:
  LogMessage(const char* file, int line, Severity severity);
  ~LogMessage();

  std::stringstream& stream() { return stream_; }
  LogMessage& SetVerboseLevel(int level) {
    verbose_level_ = level;
    return *this;
  }

  LogMessage& SetNoPrefix() {
    print_prefix_ = false;
  }

  LogMessage& OutputToStringAndLog(string* msg) { output_string_ = msg; }
  LogMessage& SetPerror();

  template <typename T>
  LogMessage& operator<<(const T& val) {
    stream_ << val;
    return *this;
  }

  LogMessage& operator<<(NoPrefixTag) {
    print_prefix_ = false;
    return *this;
  }

  LogMessage& operator<<(std::nullptr_t) {
    stream_ << "null";
    return *this;
  }

 private:
  const base::FileLocation location_;
  const Severity severity_;
  const string tid_str_;
  std::stringstream stream_;

  int verbose_level_ = 0;
  bool print_prefix_ = true;
  int perror_ = 0;
  string* output_string_ = nullptr;
};

class LogMessageNullify {
 public:
  LogMessageNullify() { }
  ~LogMessageNullify() { }
  LogMessageNullify& stream() { return *this; }
  LogMessageNullify& SetVLogLevel(int) { return *this; }
  LogMessageNullify& OutputToStringAndLog(string*) { return *this; }
  LogMessageNullify& SetPerror() { return *this; }
  template <typename T>
  LogMessageNullify& operator<<(const T&) { return *this; }
};

class LogMessageVoidify {
 public:
  LogMessageVoidify() { }
  void operator&(LogMessage&) { }
  void operator&(LogMessageNullify&) { }
};

class ScopedLog {
 public:
  ScopedLog();
  ~ScopedLog();
  void Release();
  const string& log() const { return log_; }
 private:
  string log_;
  std::unique_ptr<LogOutputDevice> device_;
};

}  // namespace logging
}  // namespace base

#define XENIA_LOGGING_INFO \
    base::logging::LogMessage(__FILE__, __LINE__, base::logging::INFO)
#define XENIA_LOGGING_WARNING \
    base::logging::LogMessage(__FILE__, __LINE__, base::logging::WARNING)
#define XENIA_LOGGING_ERROR \
    base::logging::LogMessage(__FILE__, __LINE__, base::logging::ERROR)
#define XENIA_LOGGING_FATAL \
    base::logging::LogMessage(__FILE__, __LINE__, base::logging::FATAL)

#define LOG(severity) XENIA_LOGGING_##severity

#define XENIA_LOGGING_CONDITION(condition) \
    !(condition) ? (void) 0 : ::base::logging::LogMessageVoidify() &

#define LOG_IF(severity, condition) \
    XENIA_LOGGING_CONDITION(condition) \
    XENIA_LOGGING_##severity

#define VLOG_IF(verbose_level, condition) \
    XENIA_LOGGING_CONDITION(condition) \
    XENIA_LOGGING_INFO.SetVerboseLevel(verbose_level)
#define VLOG(verbose_level) VLOG_IF(verbose_level, true)

#define CHECK(condition) \
    LOG_IF(FATAL, (!(condition))) << "Check failed: " #condition " "
#define CHECK_EQ(a, b) \
    LOG_IF(FATAL, (!((a) == (b)))) << "Check failed: " #a " (" << (a) \
        << ") == " #b " (" << (b) << ") "
#define CHECK_NE(a, b) \
    LOG_IF(FATAL, (!((a) != (b)))) << "Check failed: " #a " (" << (a) \
        << ") != " #b " (" << (b) << ") "
#define CHECK_LT(a, b) \
    LOG_IF(FATAL, (!((a) < (b)))) << "Check failed: " #a " (" << (a) \
        << ") < " #b " (" << (b) << ") "
#define CHECK_LE(a, b) \
    LOG_IF(FATAL, (!((a) <= (b)))) << "Check failed: " #a " (" << (a) \
        << ") <= " #b " (" << (b) << ") "
#define CHECK_GT(a, b) \
    LOG_IF(FATAL, (!((a) > (b)))) << "Check failed: " #a " (" << (a) \
        << ") > " #b " (" << (b) << ") "
#define CHECK_GE(a, b) \
    LOG_IF(FATAL, (!((a) >= (b)))) << "Check failed: " #a " (" << (a) \
        << ") >= " #b " (" << (b) << ") "
#define CHECK_NOTNULL(a) \
    LOG_IF(FATAL, (!((a) != nullptr))) << "Check failed: " #a " is not null "
#define CHECK_NULL(a) \
    LOG_IF(FATAL, (!((a) == nullptr))) << "Check failed: " #a " is null "
#define CHECK_STREQ(a, b) \
    LOG_IF(FATAL, (!(strcmp((a) == (b)) == 0))) << "Check failed: " #a " (\"" << (a) \
        << "\") == " #b " (\"" << (b) << "\") "
#define CHECK_STRNE(a, b) \
    LOG_IF(FATAL, (!(strcmp((a) == (b)) != 0))) << "Check failed: " #a " (\"" << (a) \
        << "\") != " #b " (\"" << (b) << "\") "
#define CHECK_STRCASEEQ(a, b) \
    LOG_IF(FATAL, (!(strcasecmp((a) == (b)) == 0))) << "Check failed: " #a " (\"" << (a) \
        << "\") == " #b " (\"" << (b) << "\") "
#define CHECK_STRCASENE(a, b) \
    LOG_IF(FATAL, (!(strcasecmp((a) == (b)) != 0))) << "Check failed: " #a " (\"" << (a) \
        << "\") != " #b " (\"" << (b) << "\") "
#define CHECK_INDEX(I, A) CHECK(I < (sizeof(A) / sizeof(A[0])))
#define CHECK_BOUND(B, A) CHECK(B <= (sizeof(A) / sizeof(A[0])))

#define PLOG(severity) XENIA_LOGGING_##severity.SetPerror()
#define PLOG_IF(severity, condition) \
    XENIA_LOGGING_CONDITION(condition) \
    XENIA_LOGGING_##severity.SetPerror()
#define LOG_TO_STRING(severity, message) \
    XENIA_LOGGING_##severity.OutputToStringAndLog(message)

#ifndef NDEBUG
  #define DCHECK(condition) CHECK(condition)
  #define DCHECK_EQ(a, b) CHECK_EQ(a, b)
  #define DCHECK_NE(a, b) CHECK_NE(a, b)
  #define DCHECK_LT(a, b) CHECK_LT(a, b)
  #define DCHECK_LE(a, b) CHECK_LE(a, b)
  #define DCHECK_GT(a, b) CHECK_GT(a, b)
  #define DCHECK_GE(a, b) CHECK_GE(a, b)
  #define DCHECK_NOTNULL(a) CHECK_NOTNULL(a)
  #define DCHECK_NULL(a) CHECK_NULL(a)
  #define DLOG LOG
  #define DLOG_IF LOG_IF
  #define DVLOG VLOG
  #define DVLOG_IF VLOG_IF
  #define DCHECK_STREQ(a, b) CHECK_STREQ(a, b)
  #define DCHECK_STRNE(a, b) CHECK_STRNE(a, b)
  #define DCHECK_STRCASEEQ(a, b) CHECK_STRCASEEQ(a, b)
  #define DCHECK_STRCASENE(a, b) CHECK_STRCASENE(a, b)
#else  // NDEBUG
  #define DCHECK(condition) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_EQ(a, b) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_NE(a, b) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_LT(a, b) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_LE(a, b) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_GT(a, b) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_GE(a, b) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_NOTNULL(a) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_NULL(a) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_STREQ(a, b) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_STRNE(a, b) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_STRCASEEQ(a, b) \
      while (false) base::logging::LogMessageNullify()
  #define DCHECK_STRCASENE(a, b) \
      while (false) base::logging::LogMessageNullify()
  #define DLOG(severity) \
      while (false) base::logging::LogMessageNullify()
  #define DLOG_IF(severity, condition) \
      while (false) base::logging::LogMessageNullify()
  #define DVLOG(verbose_level) \
      while (false) base::logging::LogMessageNullify()
  #define DVLOG_IF(verbose_level, severity) \
      while (false) base::logging::LogMessageNullify()
#endif  // NDEBUG


#endif   // BASE_LOGGING_H_
