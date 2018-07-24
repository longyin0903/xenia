#include "absl/string_view.h"
#include "gtest/gtest.h"


namespace absl {

static string_view::size_type npos = string_view::npos;

TEST(StringViewTest, Construct) {
  {
    string_view s;
    EXPECT_EQ(0, s.size());
    EXPECT_EQ(s.data(), nullptr);
  }
  {
    string str("foo");
    string_view s(str);
    EXPECT_EQ(str.data(), s.data());
    EXPECT_EQ(3, s.size());
  }
  {
    const char* p = "foo";
    string_view s(p);
    EXPECT_EQ(p, s.data());
    EXPECT_EQ(3, s.size());
  }
  {
    const char* p = "foo";
    string_view s(p, 2);
    EXPECT_EQ(p, s.data());
    EXPECT_EQ(2, s.size());
  }
}

TEST(StringViewTest, Assign) {
  string_view s1("foo");
  string_view s2("abcd");
  EXPECT_NE(s1.data(), s2.data());
  EXPECT_NE(s1.size(), s2.size());
  s2 = s1;
  EXPECT_EQ(s1.data(), s2.data());
  EXPECT_EQ(s1.size(), s2.size());
}

TEST(StringViewTest, Iterator_init) {
  {
    string_view s;
    EXPECT_TRUE(s.begin() == s.end());
    EXPECT_TRUE(s.cbegin() == s.cend());
    EXPECT_TRUE(s.rbegin() == s.rend());
    EXPECT_TRUE(s.crbegin() == s.crend());
  }
  {
    string_view s("abc");
    EXPECT_FALSE(s.begin() == s.end());
    EXPECT_EQ('a', *s.begin());
    EXPECT_FALSE(s.cbegin() == s.cend());
    EXPECT_EQ('a', *s.cbegin());
    EXPECT_FALSE(s.rbegin() == s.rend());
    EXPECT_EQ('c', *s.rbegin());
    EXPECT_FALSE(s.crbegin() == s.crend());
    EXPECT_EQ('c', *s.crbegin());
  }
}

TEST(StringViewTest, Iterator_Iteration) {
  string_view s("abc");
  {
    auto it = s.begin();
    EXPECT_EQ('a', *it);
    ++it;
    EXPECT_EQ('b', *it);
    ++it;
    EXPECT_EQ('c', *it);
    ++it;
    EXPECT_TRUE(it == s.end());
  }
  {
    auto it = s.cbegin();
    EXPECT_EQ('a', *it);
    ++it;
    EXPECT_EQ('b', *it);
    ++it;
    EXPECT_EQ('c', *it);
    ++it;
    EXPECT_TRUE(it == s.cend());
  }
  {
    auto it = s.rbegin();
    EXPECT_EQ('c', *it);
    ++it;
    EXPECT_EQ('b', *it);
    ++it;
    EXPECT_EQ('a', *it);
    ++it;
    EXPECT_TRUE(it == s.rend());
  }
  {
    auto it = s.crbegin();
    EXPECT_EQ('c', *it);
    ++it;
    EXPECT_EQ('b', *it);
    ++it;
    EXPECT_EQ('a', *it);
    ++it;
    EXPECT_TRUE(it == s.crend());
  }
}

TEST(StringViewTest, Index) {
  string_view s("abc");
  EXPECT_EQ('a', s[0]);
  EXPECT_EQ('b', s[1]);
  EXPECT_EQ('c', s[2]);
  EXPECT_EQ('a', s.at(0));
  EXPECT_EQ('b', s.at(1));
  EXPECT_EQ('c', s.at(2));
  EXPECT_EQ('a', s.front());
  EXPECT_EQ('c', s.back());
}

TEST(StringViewTest, Size) {
  string_view s("abc");
  EXPECT_EQ(3, s.size());
  EXPECT_EQ(3, s.length());
  EXPECT_EQ(3, s.max_size());
  EXPECT_FALSE(s.empty());
  EXPECT_TRUE(string_view().empty());
}

TEST(StringViewTest, RemovePrefix) {
  string_view origin("abcd");
  {
    // Remove nothing.
    string_view s = origin;
    s.remove_prefix(0);
    EXPECT_EQ(s.data(), origin.data());
    EXPECT_EQ(s.size(), origin.size());
  }
  {
    // Remove on empty.
    string_view s;
    s.remove_prefix(0);
    EXPECT_EQ(s.data(), nullptr);
    EXPECT_EQ(0, s.size());
    s.remove_prefix(1);
    EXPECT_EQ(s.data(), nullptr);
    EXPECT_EQ(0, s.size());
  }
  {
    // Remove some.
    string_view s = origin;
    s.remove_prefix(1);
    EXPECT_EQ(s.data(), origin.data() + 1);
    EXPECT_EQ(3, s.size());
  }
  {
    // Do not have enough data.
    string_view s = origin;
    s.remove_prefix(5);
    EXPECT_EQ(s.data(), origin.data() + 4);
    EXPECT_EQ(0, s.size());
  }
}

TEST(StringViewTest, RemoveSuffix) {
  string_view origin("abcd");
  {
    // Remove nothing.
    string_view s = origin;
    s.remove_suffix(0);
    EXPECT_EQ(s.data(), origin.data());
    EXPECT_EQ(s.size(), origin.size());
  }
  {
    // Remove on empty.
    string_view s;
    s.remove_suffix(0);
    EXPECT_EQ(s.data(), nullptr);
    EXPECT_EQ(0, s.size());
    s.remove_suffix(1);
    EXPECT_EQ(s.data(), nullptr);
    EXPECT_EQ(0, s.size());
  }
  {
    // Remove some.
    string_view s = origin;
    s.remove_suffix(1);
    EXPECT_EQ(s.data(), origin.data());
    EXPECT_EQ(3, s.size());
  }
  {
    // Do not have enough data.
    string_view s = origin;
    s.remove_suffix(5);
    EXPECT_EQ(s.data(), origin.data());
    EXPECT_EQ(0, s.size());
  }
}

TEST(StringViewTest, Swap) {
  const char *p = "abc";
  const char *q = "defg";
  string_view s1(p);
  string_view s2(q);
  s1.swap(s2);
  EXPECT_EQ(q, s1.data());
  EXPECT_EQ(4, s1.size());
  EXPECT_EQ(p, s2.data());
  EXPECT_EQ(3, s2.size());
}

TEST(StringViewTest, Copy) {
  char buf[10];
  string_view s("abcd");
  {
    // Copy zero byte.
    memset(buf, 0, sizeof(buf));
    EXPECT_EQ(0, s.copy(buf, 0));
    EXPECT_EQ('\0', buf[0]);
  }
  {
    // Copy some bytes.
    memset(buf, 0, sizeof(buf));
    EXPECT_EQ(3, s.copy(buf, 3));
    EXPECT_STREQ("abc", buf);
  }
  {
    // Do not have enough data.
    memset(buf, 0, sizeof(buf));
    EXPECT_EQ(4, s.copy(buf, 5));
    EXPECT_STREQ("abcd", buf);
  }
  {
    // Copy from middle
    memset(buf, 0, sizeof(buf));
    EXPECT_EQ(2, s.copy(buf, 2, 1));
    EXPECT_STREQ("bc", buf);
  }
  {
    // Copy from middle and pass the end.
    memset(buf, 0, sizeof(buf));
    EXPECT_EQ(3, s.copy(buf, 4, 1));
    EXPECT_STREQ("bcd", buf);
  }
}

TEST(StringViewTest, Substr) {
  string_view origin("abcd");
  {
    // The entire string view
    auto s = origin.substr();
    EXPECT_EQ(origin.data(), s.data());
    EXPECT_EQ(origin.size(), s.size());
  }
  {
    // From middle
    auto s = origin.substr(1);
    EXPECT_EQ(origin.data() + 1, s.data());
    EXPECT_EQ(origin.size() - 1, s.size());
  }
  {
    // From position after the end
    auto s = origin.substr(4);
    EXPECT_EQ(s.data(), nullptr);
    EXPECT_EQ(0, s.size());
  }
  {
    // From middle, and take some pieces
    auto s = origin.substr(1, 2);
    EXPECT_EQ(origin.data() + 1, s.data());
    EXPECT_EQ(2, s.size());
  }
  {
    // From middle, and pass the end
    auto s = origin.substr(1, 4);
    EXPECT_EQ(origin.data() + 1, s.data());
    EXPECT_EQ(3, s.size());
  }
  {
    // From middle, and take nothing
    auto s = origin.substr(1, 0);
    EXPECT_EQ(s.data(), nullptr);
    EXPECT_EQ(0, s.size());
  }
}

TEST(StringViewTest, Compare_StringView) {
  // For empty string view.
  EXPECT_EQ(0, string_view().compare(string_view()));
  EXPECT_EQ(-1, string_view().compare(string_view("abc")));
  EXPECT_EQ(1, string_view("abc").compare(string_view()));

  // For string view as a prefix of the other.
  EXPECT_EQ(-1, string_view("abc").compare(string_view("abcd")));
  EXPECT_EQ(1, string_view("abcd").compare(string_view("abc")));

  // general string views.
  EXPECT_EQ(0, string_view("abc").compare(string_view("abc")));
  EXPECT_EQ(-1, string_view("abc").compare(string_view("abd")));
  EXPECT_EQ(1, string_view("abc").compare(string_view("abb")));
}

TEST(StringViewTest, Compare_Others) {
  EXPECT_EQ(0, string_view("abcd").compare(1, 2, string_view("bc")));
  EXPECT_EQ(-1, string_view("abcd").compare(1, 2, string_view("bd")));
  EXPECT_EQ(1, string_view("abcd").compare(1, 2, string_view("bb")));

  EXPECT_EQ(0, string_view("abcd").compare(1, 2, string_view("xyzbcw"), 3, 2));
  EXPECT_EQ(-1, string_view("abcd").compare(1, 2, string_view("xyzbdw"), 3, 2));
  EXPECT_EQ(1, string_view("abcd").compare(1, 2, string_view("xyzbbw"), 3, 2));

  EXPECT_EQ(0, string_view("abc").compare("abc"));
  EXPECT_EQ(-1, string_view("abc").compare("abd"));
  EXPECT_EQ(1, string_view("abc").compare("abb"));

  EXPECT_EQ(0, string_view("abcd").compare(1, 2, "bc"));
  EXPECT_EQ(-1, string_view("abcd").compare(1, 2, "bd"));
  EXPECT_EQ(1, string_view("abcd").compare(1, 2, "bb"));
 
  EXPECT_EQ(0, string_view("abcd").compare(1, 2, "bcw", 2));
  EXPECT_EQ(-1, string_view("abcd").compare(1, 2, "bdw", 2));
  EXPECT_EQ(1, string_view("abcd").compare(1, 2, "bbw", 2));
}

TEST(StringViewTest, StartsWith) {
  // String views.
  // For empty string view
  EXPECT_TRUE(string_view().starts_with(string_view()));
  EXPECT_TRUE(string_view("ab").starts_with(string_view()));
  EXPECT_FALSE(string_view().starts_with(string_view("ab")));
  // For prefixes
  EXPECT_TRUE(string_view("abc").starts_with(string_view("ab")));
  EXPECT_FALSE(string_view("ab").starts_with(string_view("abc")));
  // Itself
  EXPECT_TRUE(string_view("ab").starts_with(string_view("ab")));
  // For others
  EXPECT_FALSE(string_view("abc").starts_with(string_view("abd")));

  // Single char
  EXPECT_FALSE(string_view().starts_with('a'));
  EXPECT_TRUE(string_view("a").starts_with('a'));
  EXPECT_TRUE(string_view("ab").starts_with('a'));
  EXPECT_FALSE(string_view("ba").starts_with('a'));

  // C-string.
  EXPECT_FALSE(string_view().starts_with("ab"));
  EXPECT_TRUE(string_view("ab").starts_with("ab"));
  EXPECT_TRUE(string_view("abc").starts_with("ab"));
  EXPECT_FALSE(string_view("cab").starts_with("ab"));
}

TEST(StringViewTest, EndsWith) {
  // String views.
  // For empty string view
  EXPECT_TRUE(string_view().ends_with(string_view()));
  EXPECT_TRUE(string_view("ab").ends_with(string_view()));
  EXPECT_FALSE(string_view().ends_with(string_view("ab")));
  // For suffixes
  EXPECT_TRUE(string_view("cab").ends_with(string_view("ab")));
  EXPECT_FALSE(string_view("ab").ends_with(string_view("cab")));
  // Itself
  EXPECT_TRUE(string_view("ab").ends_with(string_view("ab")));
  // For others
  EXPECT_FALSE(string_view("abc").ends_with(string_view("dbc")));

  // Single char
  EXPECT_FALSE(string_view().ends_with('a'));
  EXPECT_TRUE(string_view("a").ends_with('a'));
  EXPECT_TRUE(string_view("ba").ends_with('a'));
  EXPECT_FALSE(string_view("ab").ends_with('a'));

  // C-string.
  EXPECT_FALSE(string_view().ends_with("ab"));
  EXPECT_TRUE(string_view("ab").ends_with("ab"));
  EXPECT_TRUE(string_view("cab").ends_with("ab"));
  EXPECT_FALSE(string_view("db").ends_with("ab"));
}

TEST(StringViewTest, Find_StringView) {
  // From start
  // self is empty
  EXPECT_EQ(0, string_view().find(string_view()));
  EXPECT_EQ(npos, string_view().find(string_view("ab")));
  // Pattern is empty
  EXPECT_EQ(0, string_view("ab").find(string_view()));
  // Find matches.
  EXPECT_EQ(2, string_view("abcdecde").find(string_view("cd")));
  // Does not find.
  EXPECT_EQ(npos, string_view("abcde").find(string_view("bx")));

  // From middle
  EXPECT_EQ(3, string_view("abcabcabcd").find(string_view("abc"), 3));
  EXPECT_EQ(3, string_view("abcabcabcd").find(string_view("abc"), 1));
  EXPECT_EQ(npos, string_view("abcabd").find(string_view("abc"), 1));

  // from position passing the end
  EXPECT_EQ(npos, string_view("abc").find(string_view(), 3));
  EXPECT_EQ(npos, string_view("abc").find(string_view("abc"), 3));
}

TEST(StringViewTest, Find_SingleChar) {
  EXPECT_EQ(npos, string_view().find('a'));
  EXPECT_EQ(2, string_view("abcdce").find('c'));
  EXPECT_EQ(npos, string_view("abab").find('c'));
  EXPECT_EQ(2, string_view("abababc").find('a', 1));
  EXPECT_EQ(2, string_view("abababc").find('a', 2));
  EXPECT_EQ(npos, string_view("abxbc").find('a', 2));
}

TEST(StringViewTest, Find_Other) {
  EXPECT_EQ(2, string_view("abababc").find("abc", 1, 2));
  EXPECT_EQ(npos, string_view("abac").find("abc", 1, 2));
  EXPECT_EQ(0, string_view("abababc").find("ab"));
  EXPECT_EQ(npos, string_view("ac").find("ab"));
  EXPECT_EQ(2, string_view("abababc").find("ab", 1));
  EXPECT_EQ(npos, string_view("abac").find("ab", 1));
}

TEST(StringViewTest, RFind_StringView) {
  // From start
  // self is empty
  EXPECT_EQ(0, string_view().rfind(string_view()));
  EXPECT_EQ(npos, string_view().rfind(string_view("ab")));
  // Pattern is empty
  EXPECT_EQ(2, string_view("ab").rfind(string_view()));
  // RFind matches.
  EXPECT_EQ(5, string_view("abcdecde").rfind(string_view("cd")));
  // Does not rfind.
  EXPECT_EQ(npos, string_view("abcde").rfind(string_view("bx")));

  // From middle
  EXPECT_EQ(3, string_view("abcabcabcd").rfind(string_view("abc"), 3));
  EXPECT_EQ(3, string_view("abcabcabcd").rfind(string_view("abc"), 4));
  EXPECT_EQ(npos, string_view("abdabc").rfind(string_view("abc"), 1));

  // from position 0.
  EXPECT_EQ(0, string_view("abc").rfind(string_view(), 0));
  EXPECT_EQ(0, string_view("abc").rfind(string_view("abc"), 0));
  EXPECT_EQ(npos, string_view("abd").rfind(string_view("abc"), 0));
}

TEST(StringViewTest, RFind_SingleChar) {
  EXPECT_EQ(npos, string_view().rfind('a'));
  EXPECT_EQ(4, string_view("abcdce").rfind('c'));
  EXPECT_EQ(npos, string_view("abab").rfind('c'));
  EXPECT_EQ(2, string_view("abababc").rfind('a', 2));
  EXPECT_EQ(2, string_view("abababc").rfind('a', 3));
  EXPECT_EQ(npos, string_view("xbabc").rfind('a', 1));
}

TEST(StringViewTest, RFind_Other) {
  EXPECT_EQ(2, string_view("abababc").rfind("abc", 3, 2));
  EXPECT_EQ(npos, string_view("acab").rfind("abc", 1, 2));
  EXPECT_EQ(4, string_view("abababc").rfind("ab"));
  EXPECT_EQ(npos, string_view("ac").rfind("ab"));
  EXPECT_EQ(2, string_view("abababc").rfind("ab", 3));
  EXPECT_EQ(npos, string_view("acab").rfind("ab", 1));
}

TEST(StringViewTest, FindFirstOf_StringView) {
  // Empty string view
  EXPECT_EQ(npos, string_view().find_first_of(string_view()));
  EXPECT_EQ(npos, string_view().find_first_of(string_view("ab")));
  // Empty pattern
  EXPECT_EQ(npos, string_view("ab").find_first_of(string_view()));
  // Matches
  EXPECT_EQ(2, string_view("abcde").find_first_of(string_view("ce")));
  // Order in pattern does not matter
  EXPECT_EQ(2, string_view("abcde").find_first_of(string_view("ec")));
  // Pattern has duplicated letters.
  EXPECT_EQ(2, string_view("abcde").find_first_of(string_view("cece")));
  // Does not match.
  EXPECT_EQ(npos, string_view("abcde").find_first_of(string_view("xyz")));

  // From middle
  EXPECT_EQ(2, string_view("abababc").find_first_of(string_view("ac"), 2));
  EXPECT_EQ(2, string_view("abababc").find_first_of(string_view("ac"), 1));
  EXPECT_EQ(npos, string_view("abacabd").find_first_of(string_view("ac"), 5));
  // From position passing the end
  EXPECT_EQ(npos, string_view("abc").find_first_of(string_view(), 3));
  EXPECT_EQ(npos, string_view("abc").find_first_of(string_view("ab"), 3));
}

TEST(StringViewTest, FindFirstOf_SingleChar) {
  EXPECT_EQ(npos, string_view().find_first_of('a'));
  EXPECT_EQ(1, string_view("cabab").find_first_of('a'));
  EXPECT_EQ(npos, string_view("abab").find_first_of('c'));
  EXPECT_EQ(2, string_view("abababc").find_first_of('a', 2));
  EXPECT_EQ(2, string_view("abababc").find_first_of('a', 1));
  EXPECT_EQ(npos, string_view("abcabx").find_first_of('c', 3));
}

TEST(StringViewTest, FindFirstOf_Other) {
  EXPECT_EQ(2, string_view("abababc").find_first_of("acb", 1, 2));
  EXPECT_EQ(npos, string_view("abcxbd").find_first_of("acb", 3, 2));
  EXPECT_EQ(1, string_view("babababc").find_first_of("ac"));
  EXPECT_EQ(npos, string_view("bd").find_first_of("ac"));
  EXPECT_EQ(3, string_view("acbacb").find_first_of("ac", 2));
  EXPECT_EQ(npos, string_view("acbx").find_first_of("ac", 2));
}

TEST(StringViewTest, FindLastOf_StringView) {
  // Empty string view
  EXPECT_EQ(npos, string_view().find_last_of(string_view()));
  EXPECT_EQ(npos, string_view().find_last_of(string_view("ab")));
  // Empty pattern
  EXPECT_EQ(npos, string_view("ab").find_last_of(string_view()));
  // Matches
  EXPECT_EQ(4, string_view("abcdef").find_last_of(string_view("ce")));
  // Order in pattern does not matter
  EXPECT_EQ(4, string_view("abcdef").find_last_of(string_view("ec")));
  // Pattern has duplicated letters.
  EXPECT_EQ(4, string_view("abcdef").find_last_of(string_view("cece")));
  // Does not match.
  EXPECT_EQ(npos, string_view("abcde").find_last_of(string_view("xyz")));

  // From middle
  EXPECT_EQ(2, string_view("acababc").find_last_of(string_view("ac"), 2));
  EXPECT_EQ(2, string_view("acababc").find_last_of(string_view("ac"), 3));
  EXPECT_EQ(npos, string_view("bdxabc").find_last_of(string_view("ac"), 2));
  // From 0
  EXPECT_EQ(npos, string_view("abc").find_last_of(string_view(), 0));
  EXPECT_EQ(0, string_view("abc").find_last_of(string_view("ab"), 0));
  EXPECT_EQ(npos, string_view("xbc").find_last_of(string_view("ab"), 0));
}

TEST(StringViewTest, FindLastOf_SingleChar) {
  EXPECT_EQ(npos, string_view().find_last_of('a'));
  EXPECT_EQ(3, string_view("cabab").find_last_of('a'));
  EXPECT_EQ(npos, string_view("abab").find_last_of('c'));
  EXPECT_EQ(2, string_view("abababc").find_last_of('a', 2));
  EXPECT_EQ(2, string_view("abababc").find_last_of('a', 3));
  EXPECT_EQ(npos, string_view("abxabc").find_last_of('c', 3));
}

TEST(StringViewTest, FindLastOf_Other) {
  EXPECT_EQ(2, string_view("acababc").find_last_of("acb", 3, 2));
  EXPECT_EQ(npos, string_view("xbdabc").find_last_of("acb", 1, 2));
  EXPECT_EQ(5, string_view("acacacb").find_last_of("ac"));
  EXPECT_EQ(npos, string_view("bd").find_last_of("ac"));
  EXPECT_EQ(3, string_view("acacbxabc").find_last_of("ac", 4));
  EXPECT_EQ(npos, string_view("bxyac").find_last_of("ac", 2));
}

TEST(StringViewTest, FindFirstNotOf_StringView) {
  // Empty string view
  EXPECT_EQ(npos, string_view().find_first_not_of(string_view()));
  EXPECT_EQ(npos, string_view().find_first_not_of(string_view("abc")));
  // Empty pattern
  EXPECT_EQ(0, string_view("abc").find_first_not_of(string_view()));
  EXPECT_EQ(2, string_view("abc").find_first_not_of(string_view(), 2));
  // Match
  EXPECT_EQ(2, string_view("abcabc").find_first_not_of(string_view("ab")));
  EXPECT_EQ(2, string_view("abcabc").find_first_not_of(string_view("ba")));
  EXPECT_EQ(2, string_view("abcabc").find_first_not_of(string_view("abab")));
  // Mismatch
  EXPECT_EQ(npos, string_view("ababab").find_first_not_of(string_view("abab")));
  // From middle
  EXPECT_EQ(3, string_view("cabcabc").find_first_not_of(string_view("ab"), 3));
  EXPECT_EQ(3, string_view("cabcabc").find_first_not_of(string_view("ab"), 2));
  EXPECT_EQ(npos, string_view("cabab").find_first_not_of(string_view("ab"), 2));
  // From positions passing the end.
  EXPECT_EQ(npos, string_view("xyz").find_first_not_of(string_view("ab"), 3));
  EXPECT_EQ(npos, string_view("xyz").find_first_not_of(string_view(), 3));
}

TEST(StringViewTest, FindFirstNotOf_SingleChar) {
  EXPECT_EQ(npos, string_view().find_first_not_of('a'));
  EXPECT_EQ(1, string_view("abcbc").find_first_not_of('a'));
  EXPECT_EQ(npos, string_view("aaa").find_first_not_of('a'));
  EXPECT_EQ(3, string_view("cbabab").find_first_not_of('a', 2));
  EXPECT_EQ(npos, string_view("cbaaa").find_first_not_of('a', 2));
}

TEST(StringViewTest, FindFirstNotOf_Other) {
  EXPECT_EQ(2, string_view("abcabcabc").find_first_not_of("ab"));
  EXPECT_EQ(npos, string_view("ababab").find_first_not_of("ab"));
  EXPECT_EQ(5, string_view("abcabcabc").find_first_not_of("abx", 3, 2));
  EXPECT_EQ(npos, string_view("abcabab").find_first_not_of("abx", 3, 2));
  EXPECT_EQ(5, string_view("abcabcabc").find_first_not_of("ab", 3));
  EXPECT_EQ(npos, string_view("abcabab").find_first_not_of("ab",3));
}

TEST(StringViewTest, FindLastNotOf_StringView) {
  // Empty string view
  EXPECT_EQ(npos, string_view().find_last_not_of(string_view()));
  EXPECT_EQ(npos, string_view().find_last_not_of(string_view("abc")));
  // Empty pattern
  EXPECT_EQ(2, string_view("abc").find_last_not_of(string_view()));
  EXPECT_EQ(1, string_view("abc").find_last_not_of(string_view(), 1));
  // Match
  EXPECT_EQ(5, string_view("abcabcab").find_last_not_of(string_view("ab")));
  EXPECT_EQ(5, string_view("abcabcab").find_last_not_of(string_view("ba")));
  EXPECT_EQ(5, string_view("abcabcab").find_last_not_of(string_view("abab")));
  // Mismatch
  EXPECT_EQ(npos, string_view("ababab").find_last_not_of(string_view("abab")));
  // From middle
  EXPECT_EQ(5, string_view("abcabcabc").find_last_not_of(string_view("ab"), 5));
  EXPECT_EQ(5, string_view("abcabcabc").find_last_not_of(string_view("ab"), 6));
  EXPECT_EQ(npos, string_view("abababc").find_last_not_of(string_view("ab"), 4));
  // From 0
  EXPECT_EQ(0, string_view("xyz").find_last_not_of(string_view("ab"), 0));
  EXPECT_EQ(npos, string_view("abc").find_last_not_of(string_view("ab"), 0));
  EXPECT_EQ(0, string_view("xyz").find_last_not_of(string_view(), 0));
}

TEST(StringViewTest, FindLastNotOf_SingleChar) {
  EXPECT_EQ(npos, string_view().find_last_not_of('a'));
  EXPECT_EQ(3, string_view("bcbca").find_last_not_of('a'));
  EXPECT_EQ(npos, string_view("aaa").find_last_not_of('a'));
  EXPECT_EQ(3, string_view("ababab").find_last_not_of('a', 4));
  EXPECT_EQ(npos, string_view("aaacb").find_last_not_of('a', 2));
}

TEST(StringViewTest, FindLastNotOf_Other) {
  EXPECT_EQ(5, string_view("abcabcab").find_last_not_of("ab"));
  EXPECT_EQ(npos, string_view("ababab").find_last_not_of("ab"));
  EXPECT_EQ(5, string_view("abcabcabc").find_last_not_of("abx", 6, 2));
  EXPECT_EQ(npos, string_view("abababc").find_last_not_of("abx", 4, 2));
  EXPECT_EQ(5, string_view("abcabcabc").find_last_not_of("ab", 6));
  EXPECT_EQ(npos, string_view("abababc").find_last_not_of("ab", 4));
}

TEST(StringViewTest, LogicalComparison) {
  EXPECT_TRUE(string_view("ab") == string_view("ab"));
  EXPECT_FALSE(string_view("ab") == string_view("ac"));

  EXPECT_FALSE(string_view("ab") != string_view("ab"));
  EXPECT_TRUE(string_view("ab") != string_view("ac"));

  EXPECT_TRUE(string_view("ab") < string_view("ac"));
  EXPECT_FALSE(string_view("ab") < string_view("ab"));
  EXPECT_FALSE(string_view("ab") < string_view("aa"));

  EXPECT_TRUE(string_view("ab") <= string_view("ac"));
  EXPECT_TRUE(string_view("ab") <= string_view("ab"));
  EXPECT_FALSE(string_view("ab") <= string_view("aa"));

  EXPECT_FALSE(string_view("ab") > string_view("ac"));
  EXPECT_FALSE(string_view("ab") > string_view("ab"));
  EXPECT_TRUE(string_view("ab") > string_view("aa"));

  EXPECT_FALSE(string_view("ab") >= string_view("ac"));
  EXPECT_TRUE(string_view("ab") >= string_view("ab"));
  EXPECT_TRUE(string_view("ab") >= string_view("aa"));
}

TEST(StringViewTest, Output) {
  std::stringstream ss;
  ss << string_view();
  EXPECT_EQ("", ss.str());

  const char *p = "abcd";
  ss.str("");
  ss << string_view(p);
  EXPECT_EQ("abcd", ss.str());

  ss.str("");
  ss << string_view(p, 3);
  EXPECT_EQ("abc", ss.str());

  // align to left and pad right.
  ss.str("");
  ss.width(6);
  ss.flags(std::stringstream::left);
  ss << string_view(p, 3);
  EXPECT_EQ("abc   ", ss.str());
  // Align to right and pad left.
  ss.str("");
  ss.width(6);
  ss.flags(std::stringstream::right);
  ss << string_view(p, 3);
  EXPECT_EQ("   abc", ss.str());
}

}  // namespace absl
