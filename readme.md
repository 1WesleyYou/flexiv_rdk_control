# Flexiv 机械臂基础控制

## setup 基础配置

### RDK

请参照 [官网链接](https://rdk.flexiv.com/) 正确安装 RDK 编程工具包，
并且确保能正确运行，同时检查 `rdk_install` 文件夹需要存在于 `~` 路径下。

### CMakeLists

对于 clion 使用者，请在 cmake 设置的 cmake 选项中选择下者:
`cmake .. -DCMAKE_INSTALL_PREFIX=~/rdk_install`

同时注意将构建选项设置为 `--config Release -j 4`

### 网络问题

由于本机械臂的通讯需要用到网络通讯，所以我们首先需要确保使用网线将我们的电脑和机器人控制箱连接在一起，为了确保正确连接，我们首先使用 `ping`
指令获取到机器人的网址信息（默认网络地址是 `192.168.2.100`）同时我们需要查找自己的网络地址，即 `ifconfig` 指令(debian
系系统专用指令)

或者，我们可以专门更改代码中的部分进行快速使用，即 `robot`
初始化的时候需要用到的 `flexiv::Robot robot(robotIP, localIP);`

### 常见头文件配置

一般我们会引入以下包 :

```cpp 
#include <flexiv/Robot.hpp>
#include <flexiv/Exception.hpp>
#include <flexiv/Log.hpp>
#include <flexiv/Utility.hpp>

#include <iostream>
#include <string>
#include <thread>
```

如果工程使用到了夹爪，那么我们还需要 include 一个

```cpp
#include <flexiv/Gripper.hpp>
```

## 代码函数问题

### move(std::vector<double>)

输入是一个向量,满足各个轴转动对应的角度

### moveL()

輸入是

## 机械臂实时控制问题

我们使用的是函数 `streamJointPosition()`
这个函数基本要求是以 1kHz 的频率向机械臂发送一个控制指令，格式是 `(pos,vel,acc)` 单位是角度的弧度
其中 `pos` 等都是一个 7 维 `std::vector<double>` 向量。官方要求每个轴的变化要连续平滑，那么就会
有两种办法：插值或者降低发包频率

一般而言，当上位机和机械臂丢失通信的时候并不会直接结束程序，而是会累计丢失数量，如果数量达到20个就会报
error (可以手动清除)

那么道理上按照 19 ms 的周期发包是可以的

## 爪子控制

### [官方教程](https://rdk.flexiv.com/api/classflexiv_1_1_robot.html#a7140861dae2dfd218091d537d6ffce5c)

### 省流教程

初始化构造函数: 输入变量是对应的 robot 对象

```cpp
`// Instantiate gripper control interface
    flexiv::Gripper gripper(robot);
```

控制函数 `move()`
参数: `double width, double velocity, double force_limit`

示例代码 
```cpp
// 0.01 表示间距 (m)
// 0.1 表示速度 (m/s)
// 20 表示力 (N), 默认值 为 0
gripper.move(0.01, 0.1, 20);
// 使用进程休眠确保这一项任务完成
std::this_thread::sleep_for(std::chrono::seconds(2));
```

# 外部库

## json

由于我们需要接收外部的仿真环境发来的一个json包，我们需要对其进行解包，那么我们就会用到一些常见的网络编程的外部库，网址如下:

https://github.com/nlohmann/json/releases/tag/v3.11.3
