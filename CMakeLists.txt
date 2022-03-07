cmake_minimum_required(VERSION 3.21)
project(OpticaLab)

set(CMAKE_CXX_STANDARD 11) # set C++ standard

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib) # set where to place generated library

include_directories(includes Eigen) # directories that include header files

add_subdirectory(src)


