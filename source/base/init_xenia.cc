#include "base/init_xenia.h"

#include "base/logging.h"

namespace base {

void InitXenia(const char* app_name, int* argc, char*** argv, bool remove_flag) {
  ::base::logging::SetLogOutputDevice(
      new ::base::logging::LogOutputFileDevice(app_name));
}
}  // namespace base
