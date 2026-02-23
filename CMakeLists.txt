cmake_minimum_required(VERSION 3.16)
project(sysmon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(sysmon
    src/main.cpp
    src/config.cpp
    src/sampler.cpp
    src/averager.cpp
    src/writer.cpp
    src/query.cpp
    src/daemon.cpp
)

install(TARGETS sysmon DESTINATION bin)
