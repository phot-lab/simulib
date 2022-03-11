cmake_minimum_required(VERSION 3.21)
project(SimuLib)

set(CMAKE_CXX_STANDARD 11) # set C++ standard

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib) # Static library output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib) # Shared library output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin) # Executable file output directory

include_directories(includes) # directories that include header files

add_compile_options("-fopenmp") # Enable OpenMP features
add_compile_options("-Wall") # Turns on lots of compiler warning flags
add_compile_options("-Wno-unused-variable") # Suppress unused variable warnings
add_compile_options("-finline-functions") # Attempt to Inline All Functions, even if they are not annotated with inlining. Note: inlining trades speed for increasing of code size.

# Find MKL library on personal computer
find_package(MKL REQUIRED)
message(STATUS "${MKL_IMPORTED_TARGETS}") # Provides available list of targets based on input
include_directories(${MKL_INCLUDE})

# Necessary for linking MKL libraries
add_compile_options("-DMKL_ILP64") # MacOS or Linux
#add_compile_options("/DMKL_ILP64") # Windows

add_subdirectory(src)
add_subdirectory(test)