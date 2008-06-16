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
	~VNETexture();
	VNETexture(const VNETexture& obj);
	VNETexture& operator=(const VNETexture& rhs);
	void CopyObj(const VNETexture& obj);

	VNETexture(string fileName);
	void bindTexture();
	


};




#endif