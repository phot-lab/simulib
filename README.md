## SimuLib

数学仿真运算库，包括了光纤传输仿真软件的三个主要模块：

* 发射器模块
* 光纤模块
* 接收器模块

以下是该项目基本的目录结构：

```
SimuLib（项目root目录）
├── .clang-format（C++代码格式化配置文件）
├── CMakeLists.txt（项目总CMake配置文件）
├── includes/（头文件目录）
├── lib/（静态库和动态库）
├── src/（源代码文件目录）
└── test/（测试用例目录）
```

### 构建本项目需要安装的内容

* MKL（Intel提供的数学运算库）：[MKL下载地址](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html)
* Eigen 3.* ([Installing without using CMake](https://eigen.tuxfamily.org/dox/GettingStarted.html))

#### Prerequisites for MacOS (Big Sur or higher)
1. Install the Intel® oneAPI Base Toolkit by [m_BaseKit_p_2022.1.0.92_offline.dmg](https://registrationcenter-download.intel.com/akdlm/irc_nas/18342/m_BaseKit_p_2022.1.0.92_offline.dmg
   ).

2. Install GNU GCC 7.* by `brew install gcc@7`, see [Homebrew](https://formulae.brew.sh/formula/gcc@7#default) for detail. Then check the installation

```shell
$ gcc-7 --version
gcc-7 (Homebrew GCC 7.5.0_4) 7.5.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$ g++-7 --version
g++-7 (Homebrew GCC 7.5.0_4) 7.5.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

3. Configure `CLion > Preference > Build, Execution, Deployment > Toolchains`, add profile with C compiler (path: `/usr/local/Cellar/gcc@7/7.5.0_4/bin/gcc-7`) and C++ compiler (path: `/usr/local/Cellar/gcc@7/7.5.0_4/bin/g++-7`). Then move the new profile to the top as default.
