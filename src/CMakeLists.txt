add_library(Library STATIC tools.cpp init_gstate.cpp globals.cpp fft.cpp fiber.cpp lasersource.cpp)

add_executable(LasersourceTest lasersource.cpp)