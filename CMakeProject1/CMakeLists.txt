cmake_minimum_required(VERSION 3.10)
project(CMakeProject1)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add source files
add_executable(CMakeProject1 "CMakeProject1.cpp" "CMakeProject1.h")

# Link OpenCV libraries
target_link_libraries(CMakeProject1 ${OpenCV_LIBS})

# Set C++ standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProject1 PROPERTY CXX_STANDARD 20)
endif()

