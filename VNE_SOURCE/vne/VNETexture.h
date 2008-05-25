#ifndef VNETEXTURE
#define VNETEXTURE

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "SOIL.h"

using namespace std;

class VNETexture
{

private:
	GLuint tex_2d;

public:
	VNETexture(string fileName);
	void bindTexture();
	


};




#endif