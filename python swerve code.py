import pygame
import time
import serial

# Initialize Pygame
pygame.init()

# Initialize joystick module
pygame.joystick.init()

# Check if a joystick is connected
if pygame.joystick.get_count() == 0:
    print("No joystick detected!")
    exit()

# Get the first joystick
joystick = pygame.joystick.Joystick(0)
joystick.init()

# Get the second joystick (if present)
if pygame.joystick.get_count() > 1:
    joystick2 = pygame.joystick.Joystick(1)
    joystick2.init()
else:
    joystick2 = None

print(f"Joystick: {joystick.get_name()} initialized.")
if joystick2:
    print(f"Joystick 2: {joystick2.get_name()} initialized.")

# Setup serial communication to Arduino (adjust your port as necessary)
arduino_1 = serial.Serial('/dev/ttyUSB0', 115200)  # Arduino for module 1 and 2
arduino_2 = serial.Serial('/dev/ttyUSB1', 115200)  # Arduino for module 3 and 4
time.sleep(2)  # Allow time for Arduino to reset

# Function to record joystick positions
def record_joystick_input():
    running = True
    while running:
        # Event loop to handle events
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Get joystick axis (left stick X and Y axes)
        x_axis = joystick.get_axis(0)  # Left stick horizontal
        y_axis = joystick.get_axis(1)  # Left stick vertical

        # Get the second joystick for rotation (if it exists)
        if joystick2:
            rotation_axis = joystick2.get_axis(0)  # Right stick horizontal (rotation)
        else:
            rotation_axis = 0  # If no second joystick, set rotation to 0

        # Log joystick coordinates to file
        with open("joystick_coordinates.txt", "a") as file:
            timestamp = time.time()
            file.write(f"{timestamp}, {x_axis:.2f}, {y_axis:.2f}, {rotation_axis:.2f}\n")

        # Convert joystick X/Y into motor commands for swerve drive
        base_speed = int(255 * y_axis)
        strafe_speed = int(255 * -x_axis)
        rotation_speed = int(255 * rotation_axis)

        # Compute speeds for each motor in each module
        m1a = base_speed + rotation_speed
        m1b = strafe_speed - rotation_speed

        m2a = base_speed + rotation_speed
        m2b = -strafe_speed - rotation_speed

        m3a = base_speed + rotation_speed
        m3b = strafe_speed - rotation_speed

        m4a = base_speed + rotation_speed
        m4b = -strafe_speed - rotation_speed

        # Clamp motor values to valid range
        def clamp(val):
            return max(min(val, 255), -255)

        m1a, m1b = clamp(m1a), clamp(m1b)
        m2a, m2b = clamp(m2a), clamp(m2b)
        m3a, m3b = clamp(m3a), clamp(m3b)
        m4a, m4b = clamp(m4a), clamp(m4b)

        # Send motor commands
        command_1 = f"M1:{m1a},{m1b};M2:{m2a},{m2b}\n"
        command_2 = f"M3:{m3a},{m3b};M4:{m4a},{m4b}\n"

        arduino_1.write(command_1.encode())
        arduino_2.write(command_2.encode())

        print(f"Sent to Arduino 1: {command_1.strip()}")
        print(f"Sent to Arduino 2: {command_2.strip()}")

        pygame.time.Clock().tick(30)

    pygame.quit()

# Start recording joystick input
record_joystick_input()
