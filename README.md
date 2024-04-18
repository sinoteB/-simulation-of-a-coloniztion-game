# -simulation-of-a-coloniztion-game
Simulation of a Colonization Game

Overview:
This project is a C++ implementation of a dynamic colonization game, designed to run on Linux operating systems. It leverages POSIX threads (pthreads) to simulate a competitive environment where two teams vie to colonize a grid-based map.

Features:

    Multithreading: Utilizes pthreads to manage the concurrent operations of team members as they attempt to expand their territories.
    Resource Management: Employs mutexes to handle synchronization and prevent race conditions, ensuring that map updates are safely executed.
    Interactive Display: The grid is visually represented in the console, with distinct colors and symbols indicating the control and influence of each team over the map cells.
    End Game Logic: A supervisor thread determines when the map is fully colonized and calculates the final score to declare the winner.

Technical Details:

    Platform: Specifically tailored for Linux, using system calls and features native to this environment.
    File Handling: Manages game state through a binary file (map.bin), representing the game map where each cell’s state is persistently tracked.
    Concurrency Control: Demonstrates practical application of concurrency in software design, emphasizing thread management and synchronization.

Usage:
Run the program with command-line arguments to set the number of team members and the dimensions of the map. The program includes input validation to ensure robust operation.

This project serves as an excellent resource for understanding complex concepts such as multithreading, synchronization, and resource contention in a fun and engaging way
