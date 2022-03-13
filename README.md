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

#### Prerequisites for Ubuntu 18.04
1. (Optional) Recommend installation of Qt5 denpendencys by following this [link](https://wiki.qt.io/Building_Qt_5_from_Git)
```
$ sudo apt-get build-dep qt5-default
$ sudo apt-get install libxcb-xinerama0-dev
$ sudo apt-get install build-essential perl python git
$ sudo apt-get install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev
$ sudo apt-get install flex bison gperf libicu-dev libxslt-dev ruby
$ sudo apt-get install libxcursor-dev libxcomposite-dev libxdamage-dev libxrandr-dev libxtst-dev libxss-dev libdbus-1-dev libevent-dev libfontconfig1-dev libcap-dev libpulse-dev libudev-dev libpci-dev libnss3-dev libasound2-dev libegl1-mesa-dev gperf bison nodejs
$ sudo apt-get install libasound2-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-good1.0-dev libgstreamer-plugins-bad1.0-dev
$ sudo apt install libclang-6.0-dev llvm-6.0
```

2. GCC 7.5 should be distributed with Ubuntu 18.04 by default. Check the GCC version.
```
$ gcc --version
gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$ g++ --version
g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

3. Install Intel® oneAPI Base Toolkit by following these [link1](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html?operatingsystem=linux&distributions=aptpackagemanager), [link2](https://www.intel.com/content/www/us/en/develop/documentation/installation-guide-for-intel-oneapi-toolkits-linux/top/installation/install-using-package-managers/apt.html#apt_apt-packages)

> 1) Remove all Intel® oneAPI packages

```
$ sudo apt autoremove intel-basekit intel-hpckit intel-iotkit intel-dlfdkit intel-aikit intel-renderkit
```

> 2) Set up the Intel® oneAPI repository:

```
# create 'mkl-set-repo.sh' with following contents and execute
wget -O- https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB \
| gpg --dearmor | sudo tee /usr/share/keyrings/oneapi-archive-keyring.gpg > /dev/null

echo "deb [signed-by=/usr/share/keyrings/oneapi-archive-keyring.gpg] https://apt.repos.intel.com/oneapi all main" | sudo tee /etc/apt/sources.list.d/oneAPI.list
```

Notice: Change the 'Download from' option in 'Software and Updates > Ubuntu Software', choose a server in China, for example, mirrors.tuna.tsinghua.edu.cn

> 3) Update the repository and install

```
$ sudo apt update
$ sudo apt install intel-basekit intel-hpckit intel-iotkit intel-dlfdkit intel-aikit intel-renderkit
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following additional packages will be installed:
  cmake cmake-data intel-aikit-getting-started intel-basekit-getting-started intel-dlfdkit-getting-started
  intel-hpckit-getting-started intel-iotkit-getting-started intel-oneapi-advisor intel-oneapi-ccl-2021.5.1

... ...

  intel-oneapi-vtune intel-oneapi-vtune-eclipse-plugin-vtune intel-renderkit-getting-started libcurl4 librhash0
Suggested packages:
  cmake-doc ninja-build
The following NEW packages will be installed:
  cmake cmake-data intel-aikit intel-aikit-getting-started intel-basekit intel-basekit-getting-started intel-dlfdkit
  intel-dlfdkit-getting-started intel-hpckit intel-hpckit-getting-started intel-iotkit intel-iotkit-getting-started

... ...

  intel-oneapi-tbb-devel intel-oneapi-tbb-devel-2021.5.0 intel-oneapi-tbb-devel-2021.5.1 intel-oneapi-tensorflow
  intel-oneapi-vtune intel-oneapi-vtune-eclipse-plugin-vtune intel-renderkit intel-renderkit-getting-started libcurl4
  librhash0
0 upgraded, 122 newly installed, 0 to remove and 0 not upgraded.
Need to get 4,628 MB of archives.
After this operation, 18.2 GB of additional disk space will be used.
Do you want to continue? [Y/n]
```

4. Configure `CLion > Preference > Build, Execution, Deployment > Toolchains`, add profile with C compiler (path: `/usr/bin/gcc`) and C++ compiler (path: `/usr/bin/g++`). Then move the new profile to the top as default.

### Build instructions
1. Firstly, build the 'Library'. Check the output file under ./lib/

2. Then, build the test case one by one, such as 'MKLTest', 'FiberTest'.

3. Run the test case.
