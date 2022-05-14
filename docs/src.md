## src目录需要review的文件

| 文件路径                          | 文件说明                                                    |
| --------------------------------- | ----------------------------------------------------------- |
| src/gpu/CuFFT.cu                  | CUDA FFT implementation                                     |
| src/gpu/Tools.cu                  | Custom CUDA general tools                                   |
| src/simulib/DigitalModulator.cpp  | Digital modulator implementation                            |
| src/simulib/ElectricAmplifier.cpp | Electric amplifier implementation                           |
| src/simulib/EvaluateEye.cpp       | Evaluate eye module implementation                          |
| src/simulib/FFT.cpp               | FFT tools running on the CPU                                |
| src/simulib/Fiber.cpp             | Fiber transmit module                                       |
| src/simulib/Globals.cpp           | Global variables used by other modules                      |
| src/simulib/InitGstate.cpp        | Functions for initializing global variables                 |
| src/simulib/MatrixOperations.cpp  | Custom functions to emulate the matrix operations in MATLAB |
| src/simulib/pat2Samp.cpp          | A function used for converting pattern to samples           |
| src/simulib/Pattern.cpp           | A function used for generating random binary sequence       |
| src/simulib/RxFrontend.cpp        | Frontend receiver module                                    |
| src/simulib/Tools.cpp             | General tools, based on CPU                                 |

