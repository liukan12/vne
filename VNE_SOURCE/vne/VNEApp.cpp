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

void processHits(GLint hits, GLuint buffer[] )
{
	unsigned int i,j;
	GLuint names, *ptr;

	printf("hits = %d\n", hits );
	ptr = (GLuint* ) buffer;
	for (i = 0; i < hits; i++ )
	{
		names = *ptr;
		printf(" number of names for hit = %d\n", names);
		ptr++;
		printf(" z1 is %g;", (float) *ptr/0x7fffffff);
		ptr++;
		printf(" z2 is %g\n", (float) *ptr/0x7fffffff);
		ptr++;
		printf("	CLICK EVENT AT OBJECT NUMBER: ");
		for( j = 0; j < names; j++ )
		{
			printf("%d ", *ptr);
			ptr++;
		}
		printf("\n");
	}
}

VNEApp::VNEApp()
{
	//cout<<"Making world...\n";
	world = new VNEWorld();
	//cout<<"Making Camera Controller...\n";
	camera = new CameraControl( 0.0, 0.0, world->Getzmax(), world);
	counter = 0;
	//cout<<"Grabbing control of first object...\n";
	iControlObjIdx = 0;
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
	DemoObj = this->world->ObjList->GetObjectAt(iControlObjIdx);
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
	{	// control:	lights
		if( key == '8' )
			this->world->LightsOn();
		if( key == '9' )
			this->world->LightsOff();
	}
	{	// control: object focus
		if( key == '1' )
			iControlObjIdx = 0;
		if( key == '2' )
			iControlObjIdx = 1;
		if( key == '3' )
			iControlObjIdx = 2;
	}

}

#define BUFSIZE 512
void VNEApp::MouseCallback( int button, int state, int x, int y)
{
	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];

	if( button != GLUT_LEFT_BUTTON || state != GLUT_DOWN )
		return;
	
	glGetIntegerv(GL_VIEWPORT, viewport );
	glSelectBuffer(BUFSIZE, selectBuf );
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix( (GLdouble) x, (GLdouble) (viewport[3]-y), 5.0, 5.0, viewport );

	int w,h;
	this->camera->GetWH( &w, &h );
	gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
	
	glMatrixMode(GL_MODELVIEW);
	this->world->ObjList->DoSelection();
	glPopMatrix();
	glFlush();

	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf );
	this->camera->ResizeCallbackHandler();
	
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


