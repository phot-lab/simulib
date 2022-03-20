add_executable(Example Example.cpp)

find_library(LIBRARY Library ${PROJECT_SOURCE_DIR}/lib)

if (LIBRARY)
    target_link_libraries(Example ${LIBRARY} MKL::MKL)
else ()
    message(WARNING "Library not found, please build Library target manually.")
endif()