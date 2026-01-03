# Magister - Minibot 2025-2026
This is the program written for Magister, a robot for Lord of Robots Minibot Competition.

## Overview 
Magister is robot that competes in the Lord of Robots competition. The robot must be built no more that a 6 x 6 x 6 inches area with no weight limit. The goal of the robot is to flip and stack cubes in order to gain points in the competition.

## Demo
### Full 3d Model of Magister
<img width="1392" height="810" alt="image" src="https://github.com/user-attachments/assets/76eef526-f677-4950-ba98-b07c10bbf8a5" />

### Stacking Mechanism
https://github.com/user-attachments/assets/7ff2e29f-7b89-4c56-a9f8-5d45317b9e7f

### Flipping Mechanism (Earlier Version)
https://github.com/user-attachments/assets/2111e002-2201-480c-b425-561ce7f8bfe7

### Climb 3D Model and Actual Image
<img width="1267" height="751" alt="climb" src="https://github.com/user-attachments/assets/0bbcd5b4-168c-4a87-87b2-3d538cf9662e" />
![20260102_235407](https://github.com/user-attachments/assets/44fc6a9e-d568-4f1a-b31c-c20c2d5664a7)

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
