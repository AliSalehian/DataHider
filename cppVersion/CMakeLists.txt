cmake_minimum_required(VERSION 3.26)
project(cppVersion)

set(CMAKE_CXX_STANDARD 17)
# add OpenCV library
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# add openssl library
find_package(OpenSSL REQUIRED)

add_executable(cppVersion main.cpp
        LSB/LeastSignificantBit.cpp
        LSB/LeastSignificantBit.h
)

target_link_libraries(cppVersion ${OpenCV_LIBS} OpenSSL::SSL)