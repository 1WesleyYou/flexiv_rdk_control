//
// Created by whoami on 24-3-22.
//
#include <flexiv/Exception.hpp>
#include <flexiv/Gripper.hpp>
#include <flexiv/Log.hpp>
#include <flexiv/Robot.hpp>
#include <flexiv/Utility.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <thread>

const int MAX_INSTRUCTIONS = 30;

void loadInstruction(std::string &str) {
  std::fstream iFile;
  iFile.open("des");
  if (!iFile) {
    std::cerr << "Cant open file!" << std::endl;
  }
  return;
}

int main() {
  flexiv::Log log;
  std::string robotIP = "192.168.2.100";
  std::string localIP = "192.168.2.105"; // todo: 记得修改这里的 ip 地址
  flexiv::Robot robot(robotIP, localIP);
  flexiv::Gripper gripper(robot);
  std::string instructions[MAX_INSTRUCTIONS];
  std::fstream iFile;
  iFile.open("../des");
  if (!iFile) {
    std::cout << "cant open file!" << std::endl;
    return 0;
  }

  log.info("You Yuchen's Control Demo:");
  // 清除错误指令（如果可行）
  if (robot.isFault()) {
    log.warn("Fault occurred on robot server, trying to clear ...");
    // Try to clear the fault
    robot.clearFault();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // Check again
    if (robot.isFault()) {
      log.error("Fault cannot be cleared, exiting ...");
      return 1;
    }
    log.info("Fault on robot server is cleared");
  }

  // Enable the robot, make sure the E-stop is released before enabling
  log.info("Enabling robot ...");
  robot.enable();
  robot.setMode(flexiv::Mode::NRT_PRIMITIVE_EXECUTION);
  gripper.move(0.09, 0.1, 20);
  robot.executePrimitive("Home()");
  // Wait for reached target
  while (flexiv::utility::parsePtStates(robot.getPrimitiveStates(),
                                        "reachedTarget") != "1") {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  std::string cmds;
  while (getline(iFile, cmds)) {

    log.info("Going to destination [" + cmds + "]");

    robot.executePrimitive("MoveL(target=" + cmds + ")");

    while (flexiv::utility::parsePtStates(robot.getPrimitiveStates(),
                                          "reachedTarget") != "1") {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
  gripper.move(0.03, 0.9, 20);
  std::this_thread::sleep_for(std::chrono::seconds(2));

  robot.executePrimitive(
      "MoveL(target=0.4833 -0.3194 0.11 180 0 180 WORLD WORLD_ORIGIN)");
  while (flexiv::utility::parsePtStates(robot.getPrimitiveStates(),
                                        "reachedTarget") != "1") {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  robot.executePrimitive(
      "MoveL(target=0.4833 -0.3194 0.071 180 0 180 WORLD WORLD_ORIGIN)");

  gripper.move(0.09, 0.09, 20);
  robot.executePrimitive("Home()");
  while (flexiv::utility::parsePtStates(robot.getPrimitiveStates(),
                                        "reachedTarget") != "1") {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  robot.stop();
  //    robot.executePrimitive()
  return 0;
}