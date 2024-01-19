#include <test/test.h>
#include <str/string-util.h>

TEST_FN(_strGetExt) {
    str s   = "aaa.b";
    str ext = ".b";
    str r   = strGetExt(s);
    ASSERT_EQ(r, ext)
    r = strGetExt("");
    ASSERT(r.empty())
}

TEST_FN(_strTrim) {
    str s = " v ";
    str r = strTrim(s, false, false);
    ASSERT_EQ(r, s)
    r = strTrim(s, true, true);
    ASSERT_EQ(r, "v")
    r = strTrim(s, true, false);
    ASSERT_EQ(r, "v ")
    r = strTrim(s, false, true);
    ASSERT_EQ(r, " v")
}

TEST_FN(_strSplit) {
    std::vector<str> ss;
    str s="a  b c";
    strSplit(s, ss, ' ');
    ASSERT_EQ(ss.size(), 3)
    ASSERT_EQ(ss[0], "a")
    ASSERT_EQ(ss[1], "b")
    ASSERT_EQ(ss[2], "c")
}

int main() {
    ADD_TEST(_strGetExt);
    ADD_TEST(_strTrim);
    ADD_TEST(_strSplit);
    return runTests();
}
