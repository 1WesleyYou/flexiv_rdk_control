#include "lib/nlohmann/json.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

using json = nlohmann::json;

int main() {
  std::string line;
  json j;

  auto next = std::chrono::high_resolution_clock::now();

  while (true) {
    next += std::chrono::milliseconds(1);
    // 尝试从stdin读取一行
    if (std::getline(std::cin, line)) {
      try {
        j = json::parse(line);
        // 这里假设每一行都是一个有效的JSON，可以按照你的数据结构来访问它
        std::cout << "Received: " << j.dump() << std::endl;
      } catch (json::parse_error &e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        // 解析错误处理
      }
    }
    // 等待直到下一个周期
    std::this_thread::sleep_until(next);
  }

  return 0;
}
