file(GLOB SOURCE_FILES *.cpp)
add_library(Library STATIC ${SOURCE_FILES})
#add_library(Library STATIC tools.cpp init_gstate.cpp globals.cpp fft.cpp fiber.cpp laser_source.cpp mzmodulator.cpp ../includes/src/mzmodulator.h)

add_executable(LasersourceTest laser_source.cpp ../includes/src/laser_source.h)
add_executable(MzmodulatorTest mzmodulator.cpp)

target_link_libraries(LasersourceTest ${LIBRARY} MKL::MKL)
target_link_libraries(MzmodulatorTest ${LIBRARY} MKL::MKL)

