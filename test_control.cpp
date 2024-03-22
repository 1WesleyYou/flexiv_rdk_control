//
// Created by whoami on 24-3-22.
//
#include <flexiv/Robot.hpp>
#include <flexiv/Exception.hpp>
#include <flexiv/Log.hpp>
#include <flexiv/Utility.hpp>

#include <iostream>
#include <string>
#include <thread>

int main() {
    flexiv::Log log;
    std::string robotIP = "192.168.2.100";
    std::string localIP = "192.168.2.105"; // todo: 记得修改这里的 ip 地址
    fleiv::Robot robot(robotIP, localIP);

    log.info("You Yuchen's Control Demo:");
    return 0;
}