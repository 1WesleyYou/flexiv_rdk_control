import json
import time
import math

# Constants for the sine function
k_sineAmp = 0.035
k_sineFreq = 0.3
k_loopPeriod = 0.001  # 1 ms loop period


def simulate_joint_positions(loop_counter, init_positions):
    """Simulate joint positions based on a sine wave."""
    return [init_pos + k_sineAmp * math.sin(2 * math.pi * k_sineFreq * loop_counter * k_loopPeriod) for init_pos in
            init_positions]


def simulate_joint_velocities(loop_counter, init_positions):
    """Simulate joint velocities, derivative of the position sine wave."""
    return [k_sineAmp * 2 * math.pi * k_sineFreq * math.cos(2 * math.pi * k_sineFreq * loop_counter * k_loopPeriod) for
            _ in init_positions]


def send_packet(joint_positions, joint_velocities):
    """Generate a packet mimicking the control packet for the robot joints."""
    packet = {
        "joint_positions": joint_positions,
        "joint_velocities": joint_velocities,
        "joint_efforts": None  # Assuming no efforts data required
    }
    return json.dumps(packet)


def main():
    init_positions = [0] * 7  # Initialize joint positions for 7 joints
    loop_counter = 0

    try:
        while True:
            joint_positions = simulate_joint_positions(loop_counter, init_positions)
            joint_velocities = simulate_joint_velocities(loop_counter, init_positions)

            # Send packet
            packet = send_packet(joint_positions, joint_velocities)
            print(packet)

            # Increment loop counter
            loop_counter += 1

            # Wait for the next cycle
            time.sleep(k_loopPeriod)
    except KeyboardInterrupt:
        print("Simulation stopped.")


# Run the main function
if __name__ == "__main__":
    main()
