#include <flexiv/Robot.hpp>
#include <flexiv/Exception.hpp>
#include <flexiv/Log.hpp>
#include <flexiv/Scheduler.hpp>
#include <flexiv/Utility.hpp>

#include <iostream>
#include <string>
#include <cmath>
#include <thread>
#include <atomic>

namespace {
    constexpr double k_loopPeriod = 0.001;

    constexpr double k_sineAmp = 0.035;
    constexpr double k_sineFreq = 0.3;

    std::atomic<bool> g_schedStop = {false};
}

void printDescription() {
    std::cout << "This tutorial runs real-time joint position control to hold or sine-sweep all "
                 "robot joints."
              << std::endl
              << std::endl;
}

void printHelp() {
    // clang-format off
    std::cout << "Required arguments: [robot IP] [local IP]" << std::endl;
    std::cout << "    robot IP: address of the robot server" << std::endl;
    std::cout << "    local IP: address of this PC" << std::endl;
    std::cout << "Optional arguments: [--hold]" << std::endl;
    std::cout << "    --hold: robot holds current joint positions, otherwise do a sine-sweep" << std::endl;
    std::cout << std::endl;
    // clang-format on
}

void periodicTask(flexiv::Robot &robot, flexiv::Log &log, flexiv::RobotStates &robotStates,
                  const std::string &motionType, const std::vector<double> &initPos) {
    // Local periodic loop counter
    static unsigned int loopCounter = 0;

    try {
        // Monitor fault on robot server
        if (robot.isFault()) {
            throw flexiv::ServerException(
                    "periodicTask: Fault occurred on robot server, exiting ...");
        }

        // Read robot states
        robot.getRobotStates(robotStates);

        // Robot degrees of freedom
        size_t robotDOF = robotStates.q.size();

        // Initialize target vectors to hold position
        std::vector<double> targetPos(robotDOF, 0);
        std::vector<double> targetVel(robotDOF, 0);
        std::vector<double> targetAcc(robotDOF, 0);

        // Set target vectors based on motion type
        if (motionType == "hold") {
            targetPos = initPos;
        } else if (motionType == "sine-sweep") {
            for (size_t i = 0; i < robotDOF; ++i) {
                targetPos[i]
                        = initPos[i]
                          + k_sineAmp * sin(2 * M_PI * k_sineFreq * loopCounter * k_loopPeriod);
            }
        } else {
            throw flexiv::InputException(
                    "periodicTask: unknown motion type. Accepted motion types: hold, sine-sweep");
        }

        // Send target joint position to RDK server
        robot.streamJointPosition(targetPos, targetVel, targetAcc);

        loopCounter++;

    } catch (const flexiv::Exception &e) {
        log.error(e.what());
        g_schedStop = true;
    }
}

int main(int argc, char *argv[]) {
    // Program Setup
    // =============================================================================================
    // Logger for printing message with timestamp and coloring
    flexiv::Log log;
    std::string robotIP = "192.168.2.100";
    std::string localIP = "192.168.2.105";

    // Print description
    log.info("Tutorial description:");
    printDescription();

    // Type of motion specified by user
    std::string motionType = "";
    if (flexiv::utility::programArgsExist(argc, argv, "--hold")) {
        log.info("Robot holding current pose");
        motionType = "hold";
    } else {
        log.info("Robot running joint sine-sweep");
        motionType = "sine-sweep";
    }

    try {
        // RDK Initialization
        // =========================================================================================
        // Instantiate robot interface
        flexiv::Robot robot(robotIP, localIP);

        // Create data struct for storing robot states
        flexiv::RobotStates robotStates;

        // Clear fault on robot server if any
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

        // Wait for the robot to become operational
        while (!robot.isOperational()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        log.info("Robot is now operational");

        // Move robot to home pose
        log.info("Moving to home pose");
        robot.setMode(flexiv::Mode::NRT_PRIMITIVE_EXECUTION);
        robot.executePrimitive("Home()");

        // Wait for the primitive to finish
        while (robot.isBusy()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Real-time Joint Position Control
        // =========================================================================================
        // Switch to real-time joint position control mode
        robot.setMode(flexiv::Mode::RT_JOINT_POSITION);

        // Set initial joint positions
        robot.getRobotStates(robotStates);
        auto initPos = robotStates.q;
        log.info("Initial joint positions set to: " + flexiv::utility::vec2Str(initPos));

        // Create real-time scheduler to run periodic tasks
        flexiv::Scheduler scheduler;
        // Add periodic task with 1ms interval and highest applicable priority
        scheduler.addTask(std::bind(periodicTask, std::ref(robot), std::ref(log),
                                    std::ref(robotStates), std::ref(motionType), std::ref(initPos)),
                          "HP periodic", 1, scheduler.maxPriority());
        // Start all added tasks
        scheduler.start();

        // Block and wait for signal to stop scheduler tasks
        while (!g_schedStop) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        // Received signal to stop scheduler tasks
        scheduler.stop();

        // Wait a bit for any last-second robot log message to arrive and get printed
        std::this_thread::sleep_for(std::chrono::seconds(2));

    } catch (const flexiv::Exception &e) {
        log.error(e.what());
        return 1;
    }

    return 0;
}