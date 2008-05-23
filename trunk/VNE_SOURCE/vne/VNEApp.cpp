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
#include "CameraControl.h"

VNEApp::VNEApp()
{
	//cout<<"Making world...\n";
	world = new VNEWorld();
	//cout<<"Making Camera Controller...\n";
	camera = new CameraControl( 0.0, 0.0, world->Getzmax(), world);
	counter = 0;
	//cout<<"Grabbing control of first object...\n";
	DemoObj = this->world->ObjList->firstNode->curObj;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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
		DemoObj->TiltIncrementAxisX(0.05);
	if( key == 'y' )
		DemoObj->TiltIncrementAxisY(0.05);
	if( key == 'z' )
		DemoObj->TiltIncrementAxisZ(0.05);
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
	{	// control:  camera
		if( key == 'v' )
			this->camera->TranslateBy(0.0,0.0,0.05);
		if( key == 'c' )
			this->camera->TranslateBy(0.0,0.0,-0.05);
		if( key == 'n' )
			this->camera->TranslateBy(0.05,0.0,0.0);
		if( key == 'b' )
			this->camera->TranslateBy(-0.05,0.0,0.0);
		if( key == '.' )
			this->camera->TranslateBy(0.0,0.05,0.0);
		if( key == ',' )
			this->camera->TranslateBy(0.0,-0.05,0.0);
		if( key == 'R' )
			this->camera->ResetPosition();
		if( key == '6' )
			this->camera->AttachToObject( this->DemoObj );
		if( key == '7' )
			this->camera->Detach();
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
	this->camera->UpdateAttachedCamera();

	glColor3f(1.0, 1.0, 1.0);	

	counter++;
	counter = (counter < pow(2.0,31) ) ? counter : 0;
	

	
	glutSwapBuffers();

}

void VNEApp::ResizeCallback(int w, int h)
{
	this->camera->ResizeCallbackHandler( w, h );
}


