import subprocess
import json
import time

# 创建子进程并运行C++程序
cpp_process = subprocess.Popen(["build/sine"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)

# 循环发送包
while True:
    # 构建JSON格式的数据包
    data = {
        "joint_positions": [0.1, 0.2, 0.3, 0.4, 0.5, 0.6]  # 以示例数据为例，实际应根据需要调整
    }

    # 将数据包转换为JSON字符串
    json_data = json.dumps(data)

    try:
        # 发送JSON字符串数据到子进程的标准输入
        cpp_process.stdin.write((json_data + '\n').encode())
        cpp_process.stdin.flush()
        print("Sent:", json_data)

        # 休眠10ms，以满足10ms的发送频率
        time.sleep(0.01)

    except Exception as e:
        print("Error occurred while sending data:", e)
        break
