#include "absl/string_view.h"

#include <cassert>
#include <cstring>

namespace absl {

constexpr string_view::size_type string_view::npos;

string_view::string_view(const char* s) : ptr_(s), length_(0) {
  if (s != nullptr) length_ = static_cast<size_type>(strlen(s));
}

char string_view::at(size_type pos) const {
  assert(pos < length_);
  return *(ptr_ + pos);
}

char string_view::back() const {
  assert(length_ > 0);
  return *(ptr_ + length_ - 1);
}

string_view::size_type string_view::copy(
    char* dest, size_type n, size_type pos) const {
  if (pos > length_) { pos = length_; }
  if (n > length_ - pos) { n = length_ - pos; }
  memcpy(dest, ptr_ + pos, n);
  return n;
}

int string_view::compare(string_view s) const {
  auto res = memcmp(ptr_, s.ptr_, std::min(length_, s.length_));
  if (res != 0) { return res < 0 ? -1 : 1; }
  return length_ == s.length_ ? 0 : (length_ < s.length_ ? -1 : 1);
}

static const char* Match(
    const char* ph, size_t hlen, const char* pn, size_t nlen) {
  if (0 == nlen) { return ph; }
  if (hlen < nlen) { return nullptr; }
  const char* m;
  const char* hend = ph + hlen - nlen + 1;
  while ((m = static_cast<const char*>(memchr(ph, pn[0], hend - ph)))) {
    if (memcmp(m, pn, nlen) == 0) { return m; }
    ph = m + 1;
  }
  return nullptr;
}

string_view::size_type string_view::find(string_view s, size_type pos) const {
  if (empty() || pos >= length_) {
    if (empty() && pos == 0 && s.empty()) { return 0; }
    return npos;
  }
  auto* res = Match(ptr_ + pos, length_ - pos, s.ptr_, s.length_);
  return res ? res - ptr_ : npos;
}

string_view::size_type string_view::find(char c, size_type pos) const {
  if (empty() || pos >= length_) { return npos; }
  auto* res = static_cast<const char*>(memchr(ptr_ + pos, c, length_ - pos));
  return res ? res - ptr_ : npos;
}

string_view::size_type string_view::rfind(string_view s, size_type pos) const {
  if (length_ < s.length_) { return npos; }
  if (s.empty()) { return std::min(length_, pos); }
  const char* last = ptr_ + std::min(length_ - s.length_, pos) + s.length_;
  const char* res = std::find_end(ptr_, last, s.ptr_, s.ptr_ + s.length_);
  return res != last ? res - ptr_ : npos;
}

string_view::size_type string_view::rfind(char c, size_type pos) const {
  if (empty()) { return npos; }
  for (auto i = std::min(pos, length_ - 1); i > 0; --i) {
    if (ptr_[i] == c) { return i; }
  }
  return ptr_[0] == c ? 0 : npos;
}

namespace {
class LookupTable {
 public:
  explicit LookupTable(string_view s) {
    for (char c : s) { table_[Index(c)] = true; }
  }
  bool operator[](char c) const { return table_[Index(c)]; }

 private:
  static unsigned char Index(char c) { return static_cast<unsigned char>(c); }
  bool table_[256] = {};
};
}  // namespace

string_view::size_type string_view::find_first_of(
    string_view s, size_type pos) const {
  if (empty() || s.empty()) { return npos; }
  if (s.length_ == 1) { return find_first_of(s.front(), pos); }
  LookupTable table(s);
  for (auto i = pos; i < length_; ++i) {
    if (table[ptr_[i]]) { return i; }
  }
  return npos;
}

string_view::size_type string_view::find_last_of(
    string_view s, size_type pos) const {
  if (empty() || s.empty()) { return npos; }
  if (s.length_ == 1) { return find_last_of(s.front(), pos); }
  LookupTable table(s);
  for (auto i = std::min(pos, length_ - 1); i > 0; --i) {
    if (table[ptr_[i]]) { return i; }
  }
  return table[ptr_[0]] ? 0 : npos;
}

string_view::size_type string_view::find_first_not_of(
    string_view s, size_type pos) const {
  if (empty()) { return npos; }
  if (s.empty()) { return pos >= length_ ? npos : pos; }
  if (s.length_ == 1) { return find_first_not_of(s.front(), pos); }
  LookupTable table(s);
  for (auto i = pos; i < length_; ++i) {
    if (!table[ptr_[i]]) { return i; }
  }
  return npos;
}

string_view::size_type string_view::find_first_not_of(
    char c, size_type pos) const {
  if (empty()) { return npos; }
  for (auto i = pos; i< length_; ++i) {
    if (ptr_[i] != c) { return i; }
  }
  return npos;
}

string_view::size_type string_view::find_last_not_of(
    string_view s, size_type pos) const {
  if (empty()) { return npos; }
  if (s.empty()) { return std::min(pos, length_ - 1); }
  if (s.length_ == 1) { return find_last_not_of(s.front(), pos); }
  LookupTable table(s);
  for (auto i = std::min(pos, length_ - 1); i > 0; --i) {
    if (!table[ptr_[i]]) { return i; }
  }
  return !table[ptr_[0]] ? 0 : npos;
}

string_view::size_type string_view::find_last_not_of(
    char c, size_type pos) const {
  if (empty()) { return npos; }
  for (auto i = std::min(pos, length_ - 1); i > 0; --i) {
    if (ptr_[i] != c) { return i; }
  }
  return ptr_[0] != c ? 0 : npos;
}

static void WritePad(std::ostream& os, size_t pad) {
  char buf[32];
  memset(buf, os.fill(), sizeof(buf));
  while(pad) {
    size_t n = std::min(pad, sizeof(buf));
    os.write(buf, n);
    pad -= n;
  }
}

std::ostream& operator<<(std::ostream& os, string_view s) {
  std::ostream::sentry entry(os);
  if (entry) {
    size_t lpad = 0;
    size_t rpad = 0;
    if (static_cast<size_t>(os.width()) > s.size()) {
      size_t pad = os.width() - s.size();
      if ((os.flags() & os.adjustfield) == os.left) {
        rpad = pad;
      } else {
        lpad = pad;
      }
    }
    if (lpad) { WritePad(os, lpad); }
    os.write(s.data(), s.size());
    if (rpad) { WritePad(os, rpad); }
    os.width(0);
  }
  return os;
}

}  // namespace absl
