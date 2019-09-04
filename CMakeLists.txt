cmake_minimum_required(VERSION 3.5)
if("${CMAKE_PROJECT_NAME}" STREQUAL "" OR "${CMAKE_PROJECT_NAME}" STREQUAL "InterruptibleThread")
    project(InterruptibleThread)
endif()

set(CMAKE_CXX_STANDARD 14)
option(CODE_COVERAGE "Enable coverage reporting" OFF)
option(COMPILE_EXAMPLE "whether to compile the example executable" OFF)

add_library(InterruptibleThread InterruptibleThread.cpp)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(InterruptibleThread PRIVATE --coverage)
    target_link_libraries(InterruptibleThread INTERFACE --coverage)
endif()

if(UNIX)
    target_compile_options(InterruptibleThread PRIVATE "-fPIC")
    target_link_libraries(InterruptibleThread INTERFACE pthread)
endif()

if(COMPILE_EXAMPLE)
   add_executable(example example.cpp)
   target_link_libraries(example InterruptibleThread)
endif()
