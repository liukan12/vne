#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "VNEApp.h"
#include "VNEWorld.h"

VNEApp::VNEApp()
{
	world = new VNEWorld();
	glClearColor(0.0, 0.0 , 0.0, 0.0 );
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	counter = 0;
}

void VNEApp::IdleCallback()
{
	DisplayCallback();
}

// x & y are current mouse cursor coordinates
// x = 1, y = 1 is the upper left cornder of the window
void VNEApp::KeyboardCallback(unsigned char key, int x, int y)
{

	/*double currSpeed = this->world->DemoObj->GetSpeed( );
	double currSpin = this->world->DemoObj->GetAngVel( );
	if( key == 'w' )
		this->world->DemoObj->SetSpeed( currSpeed * 1.1 );
	if( key == 's' )
		this->world->DemoObj->SetSpeed( currSpeed * 0.9 );
	if( key == 'e' )
		this->world->DemoObj->SetAngularVelocity( currSpin * 1.1 );
	if( key == 'd' )
		this->world->DemoObj->SetAngularVelocity( currSpin * 0.9 );
	if( key == 'x' )
		this->world->DemoObj->TiltAxisTo(1.0, 0.0, 0.0);
	if( key == 'y' )
		this->world->DemoObj->TiltAxisTo(0.0, 1.0, 0.0);
	if( key == 'z' )
		this->world->DemoObj->TiltAxisTo(0.0, 0.0, 1.0);
	if( key == 'i' )
		this->world->DemoObj->TranslateBy(0.0,0.05,0.0);
	if( key == 'k' )
		this->world->DemoObj->TranslateBy(0.0,-0.05,0.0);
	if( key == 'j' )
		this->world->DemoObj->TranslateBy(-0.05,0.0,0.0);
	if( key == 'l' )
		this->world->DemoObj->TranslateBy(0.05,0.0,0.0);*/
}

void VNEApp::MouseCallback()
{

}

void VNEApp::DisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	this->world->TimeStep();
	this->world->Redraw();

	glColor3f(1.0, 1.0, 1.0);	

	counter++;
	counter = (counter < pow(2.0,31) ) ? counter : 0;
	

	
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
	gluLookAt(0.0, 0.0, this->world->Getzmax(), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


