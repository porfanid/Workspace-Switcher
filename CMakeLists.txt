cmake_minimum_required(VERSION 3.28)
project(workspaces)

# Set C++ standard
set(CMAKE_CXX_STANDARD 26)

# Find required packages
find_package(X11 REQUIRED)

# Add executable target
add_executable(workspaces main.cpp)

# Link libraries
target_link_libraries(workspaces ${X11_LIBRARIES})

# Include directories
include_directories(${X11_INCLUDE_DIR})

# Specify installation rules
install(TARGETS workspaces
        RUNTIME DESTINATION bin)
