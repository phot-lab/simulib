add_executable(Test Test.cpp)
add_executable(MatlabTest MatlabTest.cpp)
add_executable(EigenTest EigenTest.cpp)
add_executable(MKLTest MKLTest.cpp)
add_executable(FiberTest FiberTest.cpp)
add_executable(MzmodTest MzmodTest.cpp)

set(TEST_TARGETS "")
list(APPEND TEST_TARGETS Test MatlabTest EigenTest MKLTest FiberTest MzmodTest)

foreach (target IN LISTS TEST_TARGETS)
    target_link_libraries(${target} ${LIBS})
endforeach ()

# Find Matlab library on personal computer
find_package(Matlab)
if (Matlab_FOUND)
    include_directories(${Matlab_INCLUDE_DIRS})
    target_link_libraries(MatlabTest ${Matlab_ENGINE_LIBRARY} ${Matlab_DATAARRAY_LIBRARY})
endif ()