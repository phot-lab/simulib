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
├── includes（头文件目录）
├── lib（静态库和动态库）
└── src（源代码文件）
```

### 构建本项目需要安装的内容

* MKL（Intel提供的数学运算库）：[MKL下载地址](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html)

