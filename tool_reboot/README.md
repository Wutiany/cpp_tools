## main.cpp
通过定时器去设置系统关机
* `timer.wHour = WORD(22); timer.wMinute = WORD(38);` 设置关机时间的代码
## reboot.cpp
调用`win api`的关机代码部分

## Using(仅改变格式，但不能使用wingw进行编译，以下代码不可用，使用VS2019即可)
win 中可以安装 cmake 来进行编译，也可以将代码放在一个文件夹中，直接使用`main.cpp`文件生成可执行文件
### cmake使用
* 创建 build 文件夹
```shell
mkdir build
cd build
```
* 使用cmake去编译文件
```shell
cmake ..
```
* `cmake ..` 不能使用，生成的是VS的格式，则配置MinGW工具链
```shell
cmake -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="C:/MinGW/bin/make.exe" -DCMAKE_C_COMPILER="C:/MinGW/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/MinGW/bin/g++.exe" ..
```
make.exe 是改名后的的文件，为了使用 make，改名前为 `mingw32-make.exe`
* 使用make生成源文件
```shell
make
```
* 清除make文件
```shell
make clean
```