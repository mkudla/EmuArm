# EmuArm

This is a toy program that emulates some commands of the cpu.
It is modelled after early arm architecture - having 15 general purpose 32-bit registers.
Also the commands are arm-like.
I implemented it with the thought of programing my own, more exotic bit operations and checking if they can be useful for something.
You can think about commands "count leading zeros" or "count bits set to 1" as examples of recently introduced binary operations that proved to be useful, but have not been widely implemented before.

![Screenshot](https://www.github.com/mkudla/EmuArm/program.png)

## To compile:

gcc -o emuarm emuarm.c

commands:

mov register value
    - moves value to target register, ex: mov 3 2653
    
shr register shiftvalue
    - shifts target register right by shiftvalue , ex: shr 3 2

shl register shiftvalue
    - shifts target register left by shiftvalue , ex: shr 3 2

add register1 register2 targetregister
    - adds register1 register2 and puts output in targetregister, ex: add 3 2 5


sub register1 register2 targetregister
    - subs register1 register2 and puts output in targetregister, ex: sub 3 2 5

mul register1 register2 targetregister
    - multiplies register1 register2 and puts output in targetregister, ex: mul 3 2 5

xor register1 register2 targetregister
    - performs exclusive or operation between register1 and register2 and puts output in targetregister, ex: xor 3 2 5


and register1 register2 targetregister
    - performs and operation between register1 and register2 and puts output in targetregister, ex: and 3 2 5

    
Special commands:
quit - quits program
edit - inputs program, write program line by line, end program with the new line that says "end" (without parentheses) and press enter 
load - loads program from file

At this moment edit and load input program correctly, but the program is ignored either way and only interactive commands work.
Had so much fun with those that using storing program has not been implemented yet. Then I moved to the windowed version.
So interactive commands are the only working mode for now, just start typing them, i.e. mov 2, 644
