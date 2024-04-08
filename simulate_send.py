import subprocess
import time
import json
import os

# 先确保编译文件
os.system(
    'home/whoami/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/233.13135.93/bin/cmake/linux/x64/bin/cmake --build /home/whoami/Desktop/flexiv_control/cmake-build-debug --target demo --config Release -j 4')
# 替换为你的C++程序的路径
cpp_program_path = 'cmake-build-debug/demo'

# 启动C++程序并创建一个到其标准输入的管道
p = subprocess.Popen(cpp_program_path, stdin=subprocess.PIPE)

# 文件路径
file_path = 'order.txt'  # 假设你的数据文件名为data.txt

try:
    with open(file_path, 'r') as file:
        for line in file:
            line = line.replace('None', 'null')
            try:
                # 确保每一行都是有效的JSON格式
                data = json.loads(line)  # 解析JSON
                json_string = json.dumps(data)  # 将解析后的JSON重新编码为字符串
                p.stdin.write(json_string.encode() + b'\n')  # 发送JSON字符串给C++程序
                p.stdin.flush()  # 确保数据被发送
                time.sleep(0.5)  # 以10Hz的频率发送数据
            except json.JSONDecodeError as e:
                print(f"JSON解析错误: {e}")
except KeyboardInterrupt:
    p.terminate()  # 用户中断脚本时终止C++程序
