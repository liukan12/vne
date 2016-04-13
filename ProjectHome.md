VNE(VNE is Not an Engine)-This project will allow some drawing in 3D to occur that is geared for an expandable project. Future work will involve applying the engine for games, incorporating physics, and interfacing with a computer vision project. The exact specifications are still on going. The implementation will be in  C++ with possible interfaces to scripting languages.

Development Team: Peter Karasev, Ethan Rublee

Check out the current Demo!

the readme file has instructions and key commands.



---

Next Plans for Physics:

(the stuff here previously is implemented, no sense referring to it as hard or in planning)


---

Changes 29 May 2008:

Apparently I have serious issues getting a build that runs into a zip file... sorry about that. It should be good now.



---

Changes 28 May 2008:

0. Collision detection / handling with elastic conservation of momentum and energy

1. Textures are added (now hardcoded to one of the images in the data folder)

2. It detects when an object is clicked, but just prints in console which one was clicked on; no actions yet. Any suggestions for cool actions upon clicks?

3. STL Vectors are now being used instead of hand-made linked lists.

4. Added a cool brain object



---

Changes 23 May 2008:

1. Massive speedup in rotation computation

2. Added/fixed correct normals

3. Implemented Object Attaching for camera ( its funky now due to lack of background for reference )

4. Verified performance / stability with bunch of objects

5. Initialized handler for collisions


---


Changes 22 May 2008:

1. Fixed memory leak deallocating spin matrices

2. Added lighting

3. Added Cooler Objects to Demo

4. Added Normal Vectors

5. Added Camera Controller

6. Fixed scaling of velocity increments