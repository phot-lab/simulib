add_compile_options("-fopenmp") # Enable OpenMP features
add_compile_options("-Wall") # Turns on lots of compiler warning flags
add_compile_options("-finline-functions") # Attempt to Inline All Functions, even if they are not annotated with inlining. Note: inlining trades speed for increasing of code size.
add_compile_options("-DMKL_ILP64") # Necessary for linking MKL libraries on Mac OS

add_library(Library tools.cpp init_gstate.cpp globals.cpp fft.cpp)

add_executable(Test test.cpp)
add_executable(Fiber fiber.cpp)
add_executable(MKLTest mkl_test.cpp)

find_library(LIBRARY Library ${PROJECT_SOURCE_DIR}/lib)

if (LIBRARY)
    target_link_libraries(Test ${LIBRARY})
    target_link_libraries(Fiber ${LIBRARY})
endif ()

find_package(MKL REQUIRED)
message(STATUS "${MKL_IMPORTED_TARGETS}") # Provides available list of targets based on input
if (MKL_FOUND)
    include_directories(${MKL_INCLUDE})
    target_link_libraries(Test MKL::MKL)
    target_link_libraries(Fiber MKL::MKL)
    target_link_libraries(MKLTest MKL::MKL)
else ()
    message(WARNING "MKL libs not found")
endif ()