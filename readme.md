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
