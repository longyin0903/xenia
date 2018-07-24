#ifndef ABSL_STRING_VIEW_H_
#define ABSL_STRING_VIEW_H_

#include "base/using_std.h"

namespace absl {

class string_view {
 public:
  using size_type = ::std::string::size_type;
  using const_iterator = const char*;
  using iterator = const_iterator;
  using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;
  using reference = char;

  string_view() { }
  string_view(const ::std::string& s) : ptr_(s.data()), length_(s.size()) { }
  string_view(const char* s, size_type n) : ptr_(s), length_(n) { }
  string_view(const char *s);
  
  const string_view& operator=(const string_view& s) {
    ptr_ = s.ptr_;
    length_ = s.length_;
  }

  iterator begin() const { return ptr_; }
  const_iterator cbegin() const { return ptr_; }
  iterator end() const { return ptr_ + length_; }
  const_iterator cend() const { return ptr_ + length_; }
  reverse_iterator rbegin() const { return reverse_iterator(end()); }
  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(cend());
  }
  reverse_iterator rend() const { return reverse_iterator(begin()); }
  const_reverse_iterator crend() const {
    return const_reverse_iterator(cbegin());
  }

  reference operator[](size_type pos) const { return at(pos); }
  reference at(size_type pos) const;
  reference front() const { return at(0); }
  reference back() const;

  const char* data() const { return ptr_; }
  size_type length() const { return length_; }
  size_type size() const { return length_; }
  size_type max_size() const { return length_; }
  bool empty() const { return length_ == 0; }

  void remove_prefix(size_type n) {
    if (n > length_) { n = length_; }
    ptr_ += n;
    length_ -= n;
  }
  void remove_suffix(size_type n) {
    if (n > length_) { n = length_; }
    length_ -= n;
  }
  void swap(string_view &other) {
    ::std::swap(ptr_, other.ptr_);
    ::std::swap(length_, other.length_);
  }

  size_type copy(char* dest, size_type n, size_type pos = 0) const;
  string_view substr(size_type pos = 0, size_type n = npos) const {
    if (pos > length_) { pos = length_; }
    if (n > length_ - pos) { n = length_ - pos; }
    return n > 0 ? string_view(ptr_ + pos, n) : string_view();
  }
  int compare(string_view s) const;
  int compare(size_type pos, size_type n, string_view s) const {
    return substr(pos, n).compare(s);
  }
  int compare(size_type pos1, size_type n1, string_view s, size_type pos2,
              size_type n2) const {
    return substr(pos1, n1).compare(s.substr(pos2, n2));
  }
  int compare(const char* s) const { return compare(string_view(s)); }
  int compare(size_type pos, size_type n, const char* s) const {
    return substr(pos, n).compare(string_view(s));
  }
  int compare(size_type pos1, size_type n1, const char* s, size_type n2) const {
    return substr(pos1, n1).compare(string_view(s, n2));
  }

  bool starts_with(string_view s) const {
    return length_ >= s.length_ && substr(0, s.length_).compare(s) == 0;
  }
  bool starts_with(char c) const { return length_ > 0 && front() == c; }
  bool starts_with(const char* s) const { return starts_with(string_view(s)); }
  bool ends_with(string_view s) const {
    return length_ >= s.length_ &&
           substr(length_ - s.length_, s.length_).compare(s) == 0;
  }
  bool ends_with(char c) const { return length_ > 0 && back() == c; }
  bool ends_with(const char* s) const { return ends_with(string_view(s)); }

  size_type find(string_view s, size_type pos = 0) const;
  size_type find(char c, size_type pos = 0) const;
  size_type find(const char *s, size_type pos, size_type n) const {
    return find(string_view(s, n), pos);
  }
  size_type find(const char *s, size_type pos = 0) const {
    return find(string_view(s), pos);
  }

  size_type rfind(string_view s, size_type pos = npos) const;
  size_type rfind(char c, size_type pos = npos) const;
  size_type rfind(const char *s, size_type pos, size_type n) const {
    return rfind(string_view(s, n), pos);
  }
  size_type rfind(const char *s, size_type pos = npos) const {
    return rfind(string_view(s), pos);
  }

  size_type find_first_of(string_view s, size_type pos = 0) const;
  size_type find_first_of(char c, size_type pos = 0) const {
    return find(c, pos);
  }
  size_type find_first_of(const char* s, size_type pos, size_type n) const {
    return find_first_of(string_view(s, n), pos);
  }
  size_type find_first_of(const char* s, size_type pos = 0) const {
    return find_first_of(string_view(s), pos);
  }

  size_type find_last_of(string_view s, size_type pos = npos) const;
  size_type find_last_of(char c, size_type pos = npos) const {
    return rfind(c, pos);
  }
  size_type find_last_of(const char* s, size_type pos, size_type n) const {
    return find_last_of(string_view(s, n), pos);
  }
  size_type find_last_of(const char* s, size_type pos = npos) const {
    return find_last_of(string_view(s), pos);
  }

  size_type find_first_not_of(string_view s, size_type pos = 0) const;
  size_type find_first_not_of(char c, size_type pos = 0) const;
  size_type find_first_not_of(const char* s, size_type pos, size_type n) const {
    return find_first_not_of(string_view(s, n), pos);
  }
  size_type find_first_not_of(const char* s, size_type pos = 0) const {
    return find_first_not_of(string_view(s), pos);
  }

  size_type find_last_not_of(string_view s, size_type pos = npos) const;
  size_type find_last_not_of(char c, size_type pos = npos) const;
  size_type find_last_not_of(const char* s, size_type pos, size_type n) const {
    return find_last_not_of(string_view(s, n), pos);
  }
  size_type find_last_not_of(const char* s, size_type pos = npos) const {
    return find_last_not_of(string_view(s), pos);
  }

  static constexpr size_type npos = size_type(-1);

 private:
  size_type Strlen(const char*s);
  const char* ptr_ = nullptr;
  size_type length_ = 0;
};

inline bool operator==(string_view lhs, string_view rhs) {
  return lhs.compare(rhs) == 0;
}
inline bool operator!=(string_view lhs, string_view rhs) {
  return !(lhs == rhs);
}
inline bool operator<(string_view lhs, string_view rhs) {
  return lhs.compare(rhs) < 0;
}
inline bool operator<=(string_view lhs, string_view rhs) {
  return !(rhs < lhs);
}
inline bool operator>(string_view lhs, string_view rhs) {
  return rhs < lhs;
}
inline bool operator>=(string_view lhs, string_view rhs) {
  return !(lhs < rhs);
}

std::ostream& operator<<(std::ostream& os, string_view s);

}  // namespace absl

#endif  // ABSL_STRING_VIEW_H_
