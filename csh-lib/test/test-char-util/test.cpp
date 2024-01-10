#include <test/test.h>
#include <str/char-util.h>

TEST_FN(Chinese) {
//    ASSERT_EQ(isFullWidthChar('中'), true)
}

TEST_FN(En) {
    ASSERT_EQ(isFullWidthChar('v'), false)
}

int main() {
    ADD_TEST(Chinese);
    ADD_TEST(En);
    return runTests();
}
