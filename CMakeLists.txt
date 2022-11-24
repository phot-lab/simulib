cmake_minimum_required(VERSION 3.21)
project(SimuLib CXX)

option(USE_GPU "Whether use GPU for parallel computing" ON)

message(STATUS "CMake version: ${CMAKE_VERSION}")
set(CMAKE_CXX_STANDARD 11) # set C++ standard

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib) # Static library output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib) # Shared library output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin) # Executable file output directory

include_directories(includes) # directories that include header files

message(STATUS "Using the compiler: ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable") # Suppress unused variable warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finline-functions") # Attempt to Inline All Functions, even if they are not annotated with inlining. Note: inlining trades speed for increasing of code size.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") # Turns on lots of compiler warning flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -Ofast") # Enable OpenMP features
    add_link_options("-fopenmp")
endif ()

# Find MKL library on personal computer
find_package(MKL QUIET)
if (MKL_FOUND)
    message(STATUS "${MKL_IMPORTED_TARGETS}") # Provides available list of targets based on input
    include_directories(${MKL_INCLUDE})

    # Necessary for linking MKL libraries
    add_compile_options("-DMKL_ILP64") # MacOS or Linux
    #add_compile_options("/DMKL_ILP64") # Windows
else ()
    message(STATUS "MKL is not found on this machine")
endif ()

find_package(CUDAToolkit)
if (CUDAToolkit_FOUND)
    set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)
    project(SimuLib CUDA CXX)
    message(STATUS "CUDA has been found on this machine")
    message(STATUS "CUDA version: ${CUDAToolkit_VERSION}")
    include_directories(${CUDAToolkit_INCLUDE_DIRS})
    add_subdirectory(src/gpu)
    target_compile_definitions(GPULibrary PUBLIC SIMULIB_USE_GPU)
endif ()

add_subdirectory(src/simulib)

if (CUDAToolkit_FOUND)
    set(LIBS "SimuLibCPU" "SimuLibGPU")
else ()
    set(LIBS "SimuLibCPU")
endif ()

add_subdirectory(test)
add_subdirectory(example)