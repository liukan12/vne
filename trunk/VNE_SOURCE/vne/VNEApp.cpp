#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "VNEApp.h"
#include "VNEWorld.h"

VNEApp::VNEApp()
{
	world = new VNEWorld();
	glClearColor(0.0, 0.0 , 0.0, 0.0 );
	glShadeModel(GL_FLAT);
}

void VNEApp::IdleCallback()
{
	DisplayCallback();
}

void VNEApp::KeyboardCallback()
{

}

void VNEApp::MouseCallback()
{

}

void VNEApp::DisplayCallback()
{

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
	glRotatef( 90.0, 0.0, 1.0, 0.0 );
	glutWireCube(2.0);
	glPopMatrix();
	glPushMatrix();
	glRotatef( 20.0, 0.0, 1.0, 0.0 );
	glTranslatef(2.0, 0.0, 0.0);
	glRotatef(20.0, 0.0, 1.0, 0.0);
	glutWireSphere(0.2, 10, 8 ); // a planet
	glTranslatef(30.0, 30.0, 0.0 );
	glPopMatrix();
	glutSwapBuffers();

}

void VNEApp::ResizeCallback(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}




//#ifndef VNEAPP
//#define VNEAPP
//
//class VNEApp
//{
//private:
//	VNEWorld* world;
//
//public:
//	VNEApp();
//	~VNEApp();
//	void KeyboardCallback();
//	void MouseCallback();
//	void IdleCallback();
//	void DisplayCallback();
//
//
//}
//#endif