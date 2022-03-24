add_executable(Example Example.cpp)

if (LIBRARY)
    target_link_libraries(Example ${LIBS})
else ()
    message(WARNING "Library not found, please build Library target manually.")
endif()