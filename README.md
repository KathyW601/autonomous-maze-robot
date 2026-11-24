# autonomous-maze-robot
Developed firmware for a sensor-driven robot to complete automatic maze navigation
This program renders a 15x15 maze and animates a robot solving it. 
The maze is drawn on the background layer, and the robot (a green triangle) is drawn on the foreground layer. 
The robot uses a recursive Depth-First Search (DFS) algorithm to navigate from the entrance to the exit.
It uses a 2D array to store the maze and struct to store current state of robot.
The robot marks visited cells to avoid infinite loops, and uses a global stop flag to halt once the exit is found.
