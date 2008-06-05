#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "VNEMortarApp.h"
#include "VNEWorld.h"
#include "VNEObject.h"
#include "CameraControl.h"

// TODO: WRITE YOUR GAME HERE!!!

VNEMortarApp::VNEMortarApp()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void VNEMortarApp::IdleCallback()
{
	DisplayCallback();
}

// x & y are current mouse cursor coordinates
// x = 1, y = 1 is the upper left cornder of the window
void VNEMortarApp::KeyboardCallback(unsigned char key, int x, int y)
{	
	

}

void VNEMortarApp::MouseCallback()
{

}

void VNEMortarApp::DisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glutSwapBuffers();

}

void VNEMortarApp::ResizeCallback(int w, int h)
{
	//this->camera->ResizeCallbackHandler( w, h );
}


