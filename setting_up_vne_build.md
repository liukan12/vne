Setting Up a VNE Build

1. Get visual studio 2005 to follow these instructions

2. Make sure you've got GLUT. There should now be a file called _**glut**bin.zip and **glut**_src.zip.

3. To just get GLUT running, you should only need the bin folder. If you want to re-build GLUT
you can try the source file.

4. If you want to be able to step into GLUT functions, you need a debug version of glut32.dll and
thus have to build it from the source (it will be ~800kB vs. the 250kB for the release
version in the bin zip file.

5. You need to copy glut32.dll (release or debug) to C:\windows\system32\, glut32.lib to some
lib folder (such as C:\Program Files\Microsoft Visual Studio 9.0\VC\lib ) and glut.h to an
include folder such as C:\Program Files\Microsoft Visual Studio 9.0\VC\include

6. If you choose to build glut32.dll, I notice that stepping into function in debug
> only works if you keep it where you built it originally, so be careful
> as moving the folder later is not possible or a pain in the ass. It'll still work you
> just won't be able to step into the functions in the DLL.

> 7. Lastly, I am sure both of us will screw this up at some points, but:
  *  Don't check things in that are not code, data files, or self-contained demos
> That means, don't put in manifest files, intellisense database, etc, all that crap
> that is in the build directories.

