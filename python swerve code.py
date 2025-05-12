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

        # Print coordinates (for debugging)
        print(f"X: {x_axis:.2f}, Y: {y_axis:.2f}, Rotation: {rotation_axis:.2f}")

        # Convert joystick X/Y into motor commands for swerve drive
        # First joystick controls movement (forward, backward, strafe left/right)
        m1a, m1b = int(255 * y_axis), int(255 * -x_axis)  # Motor A of module 1 (X -> strafe, Y -> forward/back)
        m2a, m2b = int(255 * y_axis), int(255 * x_axis)   # Motor A of module 2 (X -> strafe, Y -> forward/back)

        # Second joystick controls rotation
        # Use rotation_axis to adjust the direction of rotation (turning the robot)
        rotation_speed = int(255 * rotation_axis)  # Scale rotation axis to PWM speed
        m1a += rotation_speed  # Add rotation to motor A of module 1
        m1b -= rotation_speed  # Subtract rotation from motor B of module 1
        m2a += rotation_speed  # Add rotation to motor A of module 2
        m2b -= rotation_speed  # Subtract rotation from motor B of module 2

        # Send motor commands to Arduino
        command_1 = f"M1:{m1a},{m1b};M2:{m2a},{m2b}\n"
        command_2 = f"M1:{m1a},{m1b};M2:{m2a},{m2b}\n"

        arduino_1.write(command_1.encode())  # Send to Arduino for modules 1 and 2
        arduino_2.write(command_2.encode())  # Send to Arduino for modules 3 and 4

        # Frame rate control
        pygame.time.Clock().tick(30)  # ~30Hz loop

    pygame.quit()

# Start recording joystick input
record_joystick_input()
