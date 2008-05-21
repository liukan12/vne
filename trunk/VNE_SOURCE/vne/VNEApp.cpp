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
#include "VNEObject.h"

VNEApp::VNEApp()
{
	world = new VNEWorld();
	glClearColor(0.0, 0.0 , 0.0, 0.0 );
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	counter = 0;
	DemoObj = this->world->ObjList->firstNode->curObj;
}

void VNEApp::IdleCallback()
{
	DisplayCallback();
}

// x & y are current mouse cursor coordinates
// x = 1, y = 1 is the upper left cornder of the window
void VNEApp::KeyboardCallback(unsigned char key, int x, int y)
{
	double currSpeed = DemoObj->GetSpeed( );
	double currSpin = DemoObj->GetAngVel( );
	if( key == 'w' )
		DemoObj->SetSpeed( currSpeed * 1.1 );
	if( key == 's' )
		DemoObj->SetSpeed( currSpeed * 0.9 );
	if( key == 'e' )
		DemoObj->SetAngularVelocity( currSpin * 1.1 );
	if( key == 'd' )
		DemoObj->SetAngularVelocity( currSpin * 0.9 );
	if( key == 'x' )
		DemoObj->TiltAxisTo(1.0, 0.0, 0.0);
	if( key == 'y' )
		DemoObj->TiltAxisTo(0.0, 1.0, 0.0);
	if( key == 'z' )
		DemoObj->TiltAxisTo(0.0, 0.0, 1.0);
	{ // control: forces
		if( key == ']' )
		{	world->EnableForce(0);
			cout<<"X-Y VORTEX ENABLED!!! \n"; }
		if( key == '[' )
		{	world->DisableForce(0);
			cout<<"X-Y VORTEX DISABLED!!! \n"; }
	}
	{  // control: acceleration
		if( key == 'i' )
			DemoObj->IncrementVelocity(0.0,0.35,0.0);
		if( key == 'k' )
			DemoObj->IncrementVelocity(0.0,-0.35,0.0);
		if( key == 'j' )
			DemoObj->IncrementVelocity(-0.35,0.0,0.0);
		if( key == 'l' )
			DemoObj->IncrementVelocity(0.35,0.0,0.0);
	}
	{   // control: translation
		if( key == 't' )
			DemoObj->TranslateBy(0.0,0.05,0.0);
		if( key == 'g' )
			DemoObj->TranslateBy(0.0,-0.05,0.0);
		if( key == 'f' )
			DemoObj->TranslateBy(-0.05,0.0,0.0);
		if( key == 'h' )
			DemoObj->TranslateBy(0.05,0.0,0.0);
	}	
	if( key == '1' )
		DemoObj = this->world->ObjList->firstNode->curObj;
	if( key == '2' )
		DemoObj = this->world->ObjList->firstNode->nextNode->curObj;
	if( key == '3' )
		DemoObj = this->world->ObjList->firstNode->nextNode->nextNode->curObj;

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


