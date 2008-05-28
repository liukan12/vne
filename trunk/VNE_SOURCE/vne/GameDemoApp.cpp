#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "GameDemoApp.h"
#include "VNEWorld.h"
#include "VNEObject.h"
#include "CameraControl.h"

// TODO: WRITE YOUR GAME HERE!!!

GameDemoApp::GameDemoApp()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void GameDemoApp::IdleCallback()
{
	DisplayCallback();
}

// x & y are current mouse cursor coordinates
// x = 1, y = 1 is the upper left cornder of the window
void GameDemoApp::KeyboardCallback(unsigned char key, int x, int y)
{	
	

}

void GameDemoApp::MouseCallback()
{

}

void GameDemoApp::DisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glutSwapBuffers();

}

void GameDemoApp::ResizeCallback(int w, int h)
{
	//this->camera->ResizeCallbackHandler( w, h );
}


