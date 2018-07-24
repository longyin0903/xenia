#ifndef BASE_INIT_XENIA_H_
#define BASE_INIT_XENIA_H_

namespace base {

void InitXenia(const char* app_name, int* argc, char*** argv,
               bool remove_flags);

}  // namespace base

#endif  // BASE_INIT_XENIA_H_
