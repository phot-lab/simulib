add_library(Library STATIC tools.cpp init_gstate.cpp globals.cpp fft.cpp fiber.cpp laser_source.cpp)

add_executable(LasersourceTest laser_source.cpp)

target_link_libraries(LasersourceTest ${LIBRARY} MKL::MKL)