cmake_minimum_required(VERSION 3.22)

project(csh-lib-tests CXX)

add_executable(test-char-util test-char-util/test.cpp)
target_link_libraries(test-char-util csh-lib)

add_executable(test-wstring-util test-wstring-util/test.cpp)
target_link_libraries(test-wstring-util csh-lib)

copy_target_to(test-char-util csh-lib)
copy_target_to(test-wstring-util csh-lib)

add_test(test-char-util test-char-util)
add_test(test-wstring-util test-wstring-util)
