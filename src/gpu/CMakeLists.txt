file(GLOB GPU_SOURCE_FILES *.cu)
add_library(GPULibrary STATIC ${GPU_SOURCE_FILES})
target_link_libraries(GPULibrary -lcublas -lcurand -lcufft)
set_target_properties(GPULibrary PROPERTIES CUDA_ARCHITECTURES "35;50;72")
