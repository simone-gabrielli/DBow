cmake_minimum_required(VERSION 3.18)
project(DBow)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library Name
set(LIBRARY_NAME DBow)

# Add include directory
include_directories(include)

# Collect source files
file(GLOB_RECURSE LIBRARY_SOURCES src/*.cpp)

# Create the library
add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})

# Link Ws2_32.lib (Only required on Windows)
if(WIN32)
    target_link_libraries(${LIBRARY_NAME} PRIVATE Ws2_32.lib)
endif()

# ==================================================
# BUILD SAMPLE (Optional)
# ==================================================

option(BUILD_SAMPLE "Build the sample application" OFF)

if(BUILD_SAMPLE)
    # Find OpenCV
    find_package(OpenCV REQUIRED)

    # Collect sample files
    file(GLOB SAMPLE_SOURCES sample/*.cpp)

    # Create sample executable
    add_executable(SampleApp ${SAMPLE_SOURCES})

    # Link with the library and OpenCV
    target_link_libraries(SampleApp PRIVATE ${LIBRARY_NAME} ${OpenCV_LIBS})

    # Include OpenCV headers
    target_include_directories(SampleApp PRIVATE ${OpenCV_INCLUDE_DIRS})

    message(STATUS "Sample application will be built.")
else()
    message(STATUS "Sample application will NOT be built. Use -DBUILD_SAMPLE=ON to enable.")
endif()
