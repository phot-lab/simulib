add_executable(Example Example.cpp)
add_executable(ExampleEye ExampleEye.cpp)
target_link_libraries(Example ${LIBS})
target_link_libraries(ExampleEye ${LIBS})