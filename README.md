# fluidsimulation-raylib-
2D SPH Fluid Simulation (C + raylib)

WHAT IS THIS PROJECT ABOUT

This project is a basic 2D fluid simulation made using particles.
Each particle follows simple physics rules so that, together, they behave like a liquid.

The simulation is written in C and uses raylib only for drawing and window handling.
It uses SPH to make the particles behave like fluids

This is meant for learning and experimenting, not for perfect or realistic physics.


FEATURES

Particle-based fluid-like motion
Simple SPH-style pressure and density
Grid / cell system for faster neighbor checks
Gravity and wall collision
Real-time rendering using raylib
Runs fully on CPU

purpose for making it:
looked fascinating


How to run the code (Setup Instructions)
1. Install raylib
Make sure raylib is installed and working.

Official website:
https://www.raylib.com

2. Project files
Your folder should contain:
main.c


3. Compile the code 

Run this single command in the terminal:
gcc src/main.c -o build/app.exe -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm

4. Run the program
build/app.exe


A window will open showing yellow particles behaving like fluid.

CONTROLS
No controls
Simulation runs automatically

POSSIBLE MODIFICATION
this project is still in progress , some modifications that i am aiming for are
1. use gpu so that i can render more particles ( currently its very slow for rendering even 1000 particles)
2. add viscosity
3. give a color according to the density to make it look more like a fluid
4. add mouse and object interactions

refrences
paper used:
https://web.archive.org/web/20250106201614if_/http://www.ligum.umontreal.ca/Clavet-2005-PVFS/pvfs.pdf
