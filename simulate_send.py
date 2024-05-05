import subprocess
import matplotlib.pyplot as plt
import numpy as np
import time
import json
import os

# 替换为你的C++程序的路径
cpp_program_path = 'cmake-build-debug/demo'

# 启动C++程序并创建一个到其标准输入的管道
p = subprocess.Popen(cpp_program_path, stdin=subprocess.PIPE)

time_cnt = 0

# 文件路径
file_path = 'order.txt'
time_span = 100  # 假设这里是 100 ms 时间延迟，具体的需要按收包实际时间差考虑
last_data = [0, 0, 0, 0, 0, 0, 0]
data_a4_last = []
data_a4_angle = []

try:
    with open(file_path, 'r') as file:
        for line in file:
            line = line.replace('None', 'null')
            try:
                # print(last_data['joint_positions'])
                data = json.loads(line)  # 解析JSON
                data_a4_angle.append(last_data[3])
                data_a4_last.append(data['joint_positions'][3])

                diff_data = []
                # 计算两个变量的差值，用于预测下一帧的走向
                for j in range(7):
                    diff_data.append((data['joint_positions'][j] - last_data[j]) / time_span)
                # print(data['joint_positions'][j])
                # print(time_span)
                # print(diff_data)
                last_data = data['joint_positions']

                for i in range(time_span):
                    local_data = [0, 0, 0, 0, 0, 0, 0]
                    for j in range(7):
                        local_data[j] = data['joint_positions'][j]  # todo: 这里可能是硬拷贝的问题，明天有空查一下
                        local_data = local_data + diff_data
                        # local_data[j] = local_data[j] + diff_data[j]
                        # data['joint_positions'][j] = data['joint_positions'][j] + diff_data[j]
                    # 确保每一行都是有效的JSON格式
                    # json_string = json.dumps(data)  # 将解析后的JSON重新编码为字符串
                    # p.stdin.write(json_string.encode() + b'\n')  # 发送JSON字符串给C++程序
                    # p.stdin.flush()  # 确保数据被发送
                    # time.sleep(0.001)  # 以1000Hz的频率发送数据
                    time_cnt = time_cnt + 0.001

                # data_a4_last.append(last_data[3])
                if time_cnt >= 20:
                    break

            except json.JSONDecodeError as e:
                print(f"JSON解析错误: {e}")
        t = np.linspace(0, 20, 20 * 10)
        plt.plot(t, data_a4_angle)
        plt.plot(t, data_a4_last)
        plt.legend()
        plt.show()
except KeyboardInterrupt:
    p.terminate()  # 用户中断脚本时终止C++程序
