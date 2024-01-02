# Replacement Door Lock Combo Keypad Controller
The controller on the Kwikset combo door lock to my house died so instead of buying a new one I did the sensible thing and spent hours designing my own ;)

![front-lock-pcb](https://github.com/james-carl-git/door-lock-keypad-controller/assets/78581339/c735624b-4679-4d0c-be4e-4cd5c2fa340c)

![back-lock-pcb](https://github.com/james-carl-git/door-lock-keypad-controller/assets/78581339/04a09f96-8e7e-4898-989c-03406821dbf9)

Below are pictures of my door lock so you can see if maybe this design would work for you. It could also be used for a custom lock project.

![Screenshot 2024-01-02 000638](https://github.com/james-carl-git/door-lock-keypad-controller/assets/78581339/305fa767-c429-4cf2-8487-2438b8a2d8ac)


Features (watch video)
- Low power consumption using MSP430 allow it to run on batteries for months/years
- Easy programming with button
- Combo stored in non-volatile memory so no need to re-program when swapping batteries
- Pizo speaker for audible button presses
- Components are inexpensive and easy to get
- PCB is a small simple 2 layer board I was able to assemble with a hot plate and some solder paste
- Mosfet H-brigde to spin lock motor forwards and backwards

https://github.com/james-carl-git/door-lock-keypad-controller/assets/78581339/344f8aa0-f60f-4c66-a90a-e0d3cf9ee638


Notes:
- My lock multiplexes the buttons into 5 connections/wires, they are as follows. You could probably modify the code if your lock is slightly different
-   Row combos: 1/2 and lock | 3/4 and 5/6 | 7/8 and 9/0
-   Column combos: 1/2 and 3/4 and 7/8 | 5/6 and 9/0 and lock
- I used Energia to program the MSP430, you'll want to put the MSP430 in DIP socket so you can remove it to program it. The PCB does not break out the programing pins
- You're going to want to make sure the voltage regulator(3.3v) has a low quiescent current if you expect the batteries to last

