cmake_minimum_required(VERSION 3.21)
project(OpticaLab)

set(CMAKE_CXX_STANDARD 11) # set C++ standard
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../lib) # set where to place generated library

# -fopenmp: Enable OpenMP features
# -Wall: Turns on lots of compiler warning flags
# -finline-functions: Attempt to Inline All Functions, even if they are not annotated with inlining. Note: inlining trades speed for increasing of code size.
set(CMAKE_CXX_FLAGS "-fopenmp -Wall -finline-functions")

include_directories(includes Eigen) # directories that include header files

add_library(Library tools.cpp init_gstate.cpp globals.cpp)

add_executable(Test test.cpp)
add_executable(Fiber fiber.cpp)

find_library(LIBRARY Library lib)

if (LIBRARY)
    target_link_libraries(Test ${LIBRARY})
    target_link_libraries(Fiber ${LIBRARY})
endif ()
