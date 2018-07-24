#ifndef BASE_FILE_LOCATION_H_
#define BASE_FILE_LOCATION_H_

#include "base/using_std.h"

namespace base {

class FileLocation {
 public:
  FileLocation(string file, int line) : file_(std::move(file)), line_(line) { }
  const char* file() const { return file_.c_str(); }
  int line() const { return line_; }
  string ToString() const;
 private:
  const string file_;
  int line_;
};

}  // namespace base

#endif  // BASE_FILE_LOCATION_H_
