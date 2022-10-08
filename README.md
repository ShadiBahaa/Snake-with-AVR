# Snake with AVR
## About
### A simulation for the known snake game using 16X2 LMB161A character LCD and 4x4 keypad.
### The application is run on AVR Atmega32 microcontroller, for any configuration changes to be suitable for another microcontroller you can refer name_CFG.h files and change whatever you want.
## Configuration
### To run the program, you can use any IDE that is suitable for running C applications, in addition to adding winavr c compiler.
## Simulation
### You can find a simulation right here in the link: https://drive.google.com/file/d/1EHXG4PUIdORS6jZc-iJhv4TUciqvmq1A/view?usp=sharing
## Features
### The snake takes the advantage of CGRAM to represent any cell in the snake as a bit on LCD, instead of utilizing the whole screen.
### Movement is controlled to be flexible at the ends of the screen, when the snake is cut to more than one part and when the direction is the opposite to the previous one
### The project simulates the whole snake as a linked list operated by a head and a tail, so the cells are connected together and deletion of one cell and adding another is smooth (Time Complexity is O(1) )
### The maximum snake length is 35 bit (considering one apple can take a whole cell) and it is more efficient than using a complete LCD cell as a bit of the snake
### Random positions are handled so that they take right places
### Very fast implementation, you will hardly notice the short periods between movements
