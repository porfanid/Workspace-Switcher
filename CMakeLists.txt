cmake_minimum_required(VERSION 3.16)
project(workspaces)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find X11 package (optional for Wayland-only systems)
find_package(X11)

# Add executable target
add_executable(workspaces main.cpp)

# If X11 is found, link it
if(X11_FOUND)
    target_link_libraries(workspaces ${X11_LIBRARIES})
    include_directories(${X11_INCLUDE_DIR})
    add_definitions(-DHAVE_X11)
endif()

# Specify installation rules
install(TARGETS workspaces
        RUNTIME DESTINATION bin)
