file(GLOB SOURCE_FILES *.cpp)
add_library(SimuLibCPU STATIC ${SOURCE_FILES})
if(CUDAToolkit_FOUND)
    add_library(SimuLibGPU STATIC ${SOURCE_FILES})
    target_compile_definitions(SimuLibGPU PUBLIC SIMULIB_USE_GPU)
    target_link_libraries(SimuLibGPU GPULibrary)
endif()