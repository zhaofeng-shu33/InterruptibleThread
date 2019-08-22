cmake_minimum_required(VERSION 3.5)
project(InterruptibleThread)
option(CODE_COVERAGE "Enable coverage reporting" OFF)
option(COMPILE_EXAMPLE "whether to compile the example executable" OFF)

add_library(InterruptibleThread InterruptibleThread.cpp)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(InterruptibleThread PRIVATE --coverage)
    target_link_libraries(InterruptibleThread INTERFACE --coverage)
endif()

if(UNIX)
    target_link_libraries(InterruptibleThread INTERFACE pthread)
endif()

if(COMPILE_EXAMPLE)
   add_executable(example example.cpp)
   target_link_libraries(example InterruptibleThread)
endif()