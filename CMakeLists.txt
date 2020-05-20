cmake_minimum_required (VERSION 3.8)

project ("MessageBus")

set(CMAKE_CXX_STANDARD 17)

add_executable (test "main.cpp" "MessageBus.cpp" "_MessageBus.cpp" "Semaphore.cpp")
