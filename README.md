# Magister - Minibot 2025-2026
This is the program written for Magister, a robot for Lord of Robots Minibot Competition.

## Overview 
Magister is robot that competes in the Lord of Robots competition. The robot must be built no more that a 6 x 6 x 6 inches area with no weight limit. The goal of the robot is to flip and stack cubes in order to gain points in the competition.

## Demo
<img width="1392" height="810" alt="image" src="https://github.com/user-attachments/assets/76eef526-f677-4950-ba98-b07c10bbf8a5" />

## Capabilities
- An forklift mechanism that grabs cubes and stacks them
- A flipper for flipping game pieces
- A pair of climbing mechanism that can be used on an edge
- An arcade drive for movement

## Hardware
- Microcontroller: LoR Core Version 3
- Motors: 4x N20 Motors, 2x MG950 180 Degree Rotation Servos, 1x Continuous Rotation Servo 
- Power: 2x 8V Batteries
- Parts: 3D-printed PLA

- ## Software
- Language: C++
- Platform: Arduino IDE

- ## How It Works
1. The robot is controlled using a controller to move
2. Move around the field with 45 degree lift with the flipped in order to decrease the amount of friction with the ground
3. The flipping mechanism is used to flip cubes using the 90 degree flip
4. Use the other side of the bot to activate the forklift and stack the cube on top of another one

## Future Improvements
- Add a 180 degree position for the flipper
- Stronger connection with the forks (tend to break quite easily)
