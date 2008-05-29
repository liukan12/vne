VNE Verion 0.4

Key Commands:

1	Control Object 1
2	Control Object 2
3	Control Object 3
4	Control Object 4

w	Speed up
s	Slow Down

e	Speed Up Spin
d	Slow Down Spin

x	Spin about x-axis
y	Spin about y-axis
z	Spin about z-axis

]	Enable Vortex (spiral to origin)
[	Disable Vortex

i	accelerate up
k	accelerate down
j	accelerate left
l	accelerate right

t	move up
g	move down
f	move left
h	move right

v	zoom camera in	(z-axis)
c	zoom camera out (z-axis)

6   attach camera to object
7   detach camera from object

b,n, ,, . : control x&y position of camera (this control is funky now)

R	(shift-r) reset camera position (z = 5, pointed down at origin)

8	enable lights (disable colors)
9	enable colors (disable lights)

Note: you can change the files in the \data directory to be some other images,
just keep the names the same... later you'll be able to set file names.

------------------------------------------------------------------------------------------------
Changes 28 May 2008:
0. Collision detection / handling with elastic conservation of momentum and energy

1. Textures are added (now hardcoded to one of the images in the data folder)

2. It detects when an object is clicked, but just prints in console which one was clicked on; no actions yet. Any suggestions for cool actions upon clicks?

3. STL Vectors are now being used instead of hand-made linked lists.

4. Collisions are detected but the user doesn't know at the moment...

5. Added a cool brain object 

-----------------------------------------------------------------------------------------------
Changes 25 May 2008:
1. Added image textures

2. Added some image handling code

3. Added texture class

4. Added texturability for objects

5. Started work on (linear) momentun & (kinetic) energy conservation for elastic collisions

-----------------------------------------------------------------------------------------------
Changes 24 May 2008:

1. Fixed bugs in camera orientation when attached/detaching

2. Added textures for walls

3. Added enable/disable lights ability

-----------------------------------------------------------------------------------------------
Changes 23 May 2008:

1. Massive speedup in rotation computation

2. Added/fixed correct normals

3. Implemented Object Attaching for camera ( its funky now due to lack of background for reference )

4. Verified performance / stability with bunch of objects

5. Initialized handler for collisions

-----------------------------------------------------------------------------------------------

Changes 22 May 2008:

1. Fixed memory leak deallocating spin matrices

2. Added lighting

3. Added Cooler Objects to Demo

4. Added Normal Vectors

5. Added Camera Controller

6. Fixed scaling of velocity increments