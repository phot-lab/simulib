# -fopenmp: Enable OpenMP features
# -Wall: Turns on lots of compiler warning flags
# -finline-functions: Attempt to Inline All Functions, even if they are not annotated with inlining. Note: inlining trades speed for increasing of code size.
set(CMAKE_CXX_FLAGS "-fopenmp -Wall -finline-functions")

add_library(Library tools.cpp init_gstate.cpp globals.cpp)

add_executable(Test test.cpp)
add_executable(Fiber fiber.cpp)

find_library(LIBRARY Library ${PROJECT_SOURCE_DIR}/lib)

if (LIBRARY)
    target_link_libraries(Test ${LIBRARY})
    target_link_libraries(Fiber ${LIBRARY})
endif ()