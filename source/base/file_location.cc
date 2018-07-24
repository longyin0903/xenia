#include "base/file_location.h"

namespace base {

string FileLocation::ToString() const {
  char buf[16];
  memset(buf, 0, sizeof(buf));
  snprintf(buf, sizeof(buf), ":%d", line_);
  return file_ + buf;
}

}  // namespace base
