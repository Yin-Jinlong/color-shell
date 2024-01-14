cmake_minimum_required(VERSION 3.22)

project(csh-lib-tests CXX)


add_executable(test-string-util test-string-util/test.cpp)
target_link_libraries(test-string-util csh-lib)

copy_target_to(test-string-util csh-lib)

add_test(test-string-util test-string-util)
