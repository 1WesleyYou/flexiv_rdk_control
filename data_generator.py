import subprocess
import time
import json
import random

# 替换为你的C++程序的路径
cpp_program_path = 'cmake-build-debug/demo'

# 启动C++程序并创建一个到其标准输入的管道
p = subprocess.Popen(cpp_program_path, stdin=subprocess.PIPE)

# 初始化位置和速度
current_positions = [0.0 for _ in range(7)]
current_velocities = [0.0 for _ in range(7)]


def update_data():
    """更新模拟数据包中的位置和速度"""
    global current_positions, current_velocities
    for i in range(7):
        position_change = random.uniform(-0.01, 0.01)  # 小幅度调整
        velocity_change = random.uniform(-0.005, 0.005)  # 小幅度调整
        current_positions[i] += position_change
        current_velocities[i] += velocity_change


def generate_data():
    """生成模拟数据包"""
    data = {
        'joint_positions': current_positions,
        'joint_velocities': current_velocities,
        'joint_efforts': None
    }
    return json.dumps(data) + '\n'  # JSON格式，以换行符结束


try:
    while True:
        update_data()  # 更新数据以模拟连续变化
        data = generate_data()
        p.stdin.write(data.encode())
        p.stdin.flush()  # 确保数据被发送
        time.sleep(0.1)  # 以10Hz频率发送
except KeyboardInterrupt:
    p.terminate()  # 用户中断脚本时终止C++程序
