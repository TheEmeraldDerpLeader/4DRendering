# 4DRendering
A 4D rendering thing I've been working on.
I'll need to write this README some other time, but for now I'll say this: 

--NOTE: THIS IS OUTDATED--

This project is a 4D renderer that takes an approach known as the cross-sectional approach.
You may have heard of something called "4D projections" before; This is completely different from a projection. Put simply, the cross-sectional approach takes a "slice" of the 4D world and uses that slice to render what you see. This slice is actually a 3D space rather than a 2D plane. 4D objects have a 3D surfaces called cells. These cells create a face when they cross your viewing slice. Each cell of a 4D object will only create one face. For example, a pentahedron (4D tetrahedron) has 5 cells and will appear with at most 5 faces. This causes interesting shapes to form from rotation and moving in 4D.

In the executable, there are two shapes: a tesseract (aka a 4D cube) and a pentahedron (aka a 4-simplex or a 4D tetrahedron). These shapes are both known as 4D platonic solids. wikipedia:https://en.wikipedia.org/wiki/Regular_4-polytope

Controls: This controls similar to a fps. WASD for forward, left, back, right. Q and E for down and up respectively. You can hold left shift to move faster.
Here is where 4D comes into play. The keys 1 and 3 are used for -w and +w respectively. Left control is used to rotate along the XW and YW planes. Simply hold left control while moving your mouse and you will rotate along these planes.
Some other but VERY IMPORTANT CONTROLS: press ESC TO EXIT THE PROGRAM and press the L KEY TO RELEASE YOUR MOUSE POINTER. THE L KEY IS NOT DESIGNED FOR CONVIENCE: THE PROGRAM WILL CONSTANTLY LOCK AND RELEASE YOUR MOUSE IF YOU HOLD DOWN L.

Some useful links:
http://miegakure.com/
https://www.youtube.com/watch?v=4URVJ3D8e8k

One last note: the colors of the faces aren't set. The depend on when each cell is rendered. for example, a green cell will turn blue if the white, red, green, or blue cell goes out of vision because the green cell was rendered 4th.
