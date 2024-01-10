#pragma once

#include <predef.h>
#include <iostream>
#include <vector>

typedef void (*TestFn)(str &error);

struct TestCase {
    str    name;
    TestFn fn;
};

#define TEST_FN(name) void test##name(str &error)

#define ASSERT(b) if (!b) {\
error="assert failed";\
}

#define ASSERT_EQ(a, b) if ((a)!=(b)) {\
error=std::format("'{}' != '{}'", a, b);\
}

std::vector<TestCase> tests;

#define ADD_TEST(name) tests.push_back({"test"#name, test##name})

static inline int runTests() {
    int       fc = 0;
    int       pc = 0;
    for (auto &tc: tests) {
        try {
            str err;
            tc.fn(err);
            if (err.empty()) {
                std::cout << "\x1b[32m" << tc.name << ": ok " << std::endl;
                pc++;
            } else {
                fc++;
                std::cout << "\x1b[31m" << tc.name << ": failed" << std::endl;
                std::cout << err << std::endl;
            }
        } catch (std::exception &e) {
            fc++;
            std::cout << "\x1b[31m" << tc.name << ": failed" << std::endl;
            std::cout << e.what() << std::endl;
        }
    }
    std::cout << "\x1b[32mpassed:" << pc << std::endl;
    std::cout << "\x1b[31mfailed:" << fc << std::endl;
    std::cout << "\x1b[36mpass:" << 100.0 * pc / (pc + fc) << "%" << std::endl;
    return fc;
}
