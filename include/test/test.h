#pragma once

#include <predef.h>
#include <iostream>
#include <vector>

typedef void (*TestFn)(wstr &error);

struct TestCase {
    wstr   name;
    TestFn fn;
};

#define TEST_FN(name) void test##name(wstr &error)

#define ASSERT(b) if (!b) {\
error=L"assert failed";\
}

#define ASSERT_EQ(a, b) if ((a)!=(b)) {\
error=std::format(L"'{}' != '{}'", a, b);\
}

std::vector<TestCase> tests;

#define ADD_TEST(name) tests.push_back({L"test"#name, test##name})

static inline int runTests() {
    int       fc = 0;
    int       pc = 0;
    for (auto &tc: tests) {
        try {
            wstr err;
            tc.fn(err);
            if (err.empty()) {
                std::wcout << L"\x1b[32m" << tc.name << ": ok " << std::endl;
                pc++;
            } else {
                fc++;
                std::wcout << L"\x1b[31m" << tc.name << L": failed" << std::endl;
                std::wcout << err << std::endl;
            }
        } catch (std::exception &e) {
            fc++;
            std::wcout << L"\x1b[31m" << tc.name << L": failed" << std::endl;
            std::wcout << e.what() << std::endl;
        }
    }
    std::wcout << L"\x1b[32mpassed:" << pc << std::endl;
    std::wcout << L"\x1b[31mfailed:" << fc << std::endl;
    std::wcout << L"\x1b[36mpass:" << 100.0 * pc / (pc + fc) << L"%" << std::endl;
    return fc;
}
