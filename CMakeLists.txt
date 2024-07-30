cmake_minimum_required(VERSION 3.28)
project(workspaces)

set(CMAKE_CXX_STANDARD 26)

find_package(X11 REQUIRED)

add_executable(workspaces main.cpp)
target_link_libraries(workspaces ${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})
