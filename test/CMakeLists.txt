add_executable(Test Test.cpp)
add_executable(MatlabTest MatlabTest.cpp)
add_executable(EigenTest EigenTest.cpp)
add_executable(MKLTest MKLTest.cpp)
add_executable(FiberTest FiberTest.cpp)
add_executable(MzmodTest MzmodTest.cpp)
add_executable(FFTTest FFTTest.cpp)
add_executable(ParMatTest ParMatTest.cpp)

set(TEST_TARGETS "")
list(APPEND TEST_TARGETS Test MatlabTest EigenTest MKLTest FiberTest MzmodTest FFTTest ParMatTest)

foreach (target IN LISTS TEST_TARGETS)
    target_link_libraries(${target} ${LIBS})
endforeach ()

if (MKL_FOUND)
    target_link_libraries(MKLTest MKL::MKL)
endif ()

# Find Matlab library on personal computer
find_package(Matlab)
if (Matlab_FOUND)
    include_directories(${Matlab_INCLUDE_DIRS})
    target_link_libraries(MatlabTest ${Matlab_ENGINE_LIBRARY} ${Matlab_DATAARRAY_LIBRARY})
endif ()