#include <test/test.h>
#include "str/wstring-util.h"

TEST_FN(_wstrGetExt) {
    wstr s   = L"aaa.b";
    wstr ext = L".b";
    wstr r   = wstrGetExt(s);
    ASSERT_EQ(r, ext)
    r = wstrGetExt(L"");
    ASSERT(r.empty())
}

TEST_FN(_wstrTrim) {
    wstr s = L" v ";
    wstr r = wstrTrim(s, false, false);
    ASSERT_EQ(r, s)
    r = wstrTrim(s, true, true);
    ASSERT_EQ(r, L"v")
    r = wstrTrim(s, true, false);
    ASSERT_EQ(r, L"v ")
    r = wstrTrim(s, false, true);
    ASSERT_EQ(r, L" v")
}

TEST_FN(_wstrSplit) {
    std::vector<wstr> ss;
    wstr s=L"a  b c";
    wstrSplit(s, ss, ' ');
    ASSERT_EQ(ss.size(), 3)
    ASSERT_EQ(ss[0], L"a")
    ASSERT_EQ(ss[1], L"b")
    ASSERT_EQ(ss[2], L"c")
}

int main() {
    ADD_TEST(_wstrGetExt);
    ADD_TEST(_wstrTrim);
    ADD_TEST(_wstrSplit);
    return runTests();
}
