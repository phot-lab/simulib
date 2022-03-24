cmake_minimum_required(VERSION 3.21)
project(SimuLib CXX)

set(CMAKE_CXX_STANDARD 11) # set C++ standard

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib) # Static library output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib) # Shared library output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin) # Executable file output directory

include_directories(includes) # directories that include header files

message(STATUS "Using the compiler: ${CMAKE_CXX_COMPILER_ID}")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options("-Wno-unused-variable") # Suppress unused variable warnings
    add_compile_options("-Wno-unused-but-set-variable")
    add_compile_options("-finline-functions") # Attempt to Inline All Functions, even if they are not annotated with inlining. Note: inlining trades speed for increasing of code size.
    add_compile_options("-Wall") # Turns on lots of compiler warning flags
    add_compile_options("-fopenmp") # Enable OpenMP features
    add_link_options("-fopenmp")
endif()

# Find MKL library on personal computer
find_package(MKL)
if (MKL_FOUND)
    message(STATUS "${MKL_IMPORTED_TARGETS}") # Provides available list of targets based on input
    include_directories(${MKL_INCLUDE})

    # Necessary for linking MKL libraries
    add_compile_options("-DMKL_ILP64") # MacOS or Linux
    #add_compile_options("/DMKL_ILP64") # Windows
endif ()

add_subdirectory(src)
find_library(LIBRARY Library ${PROJECT_SOURCE_DIR}/lib)

if (MKL_FOUND)
    set(LIBS ${LIBRARY} MKL::MKL)
else ()
    set(LIBS ${LIBRARY})
endif ()

add_subdirectory(test)
add_subdirectory(example)