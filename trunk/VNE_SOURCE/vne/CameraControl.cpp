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


CameraControl::CameraControl(VNEWorld* aWorld)
{
	atx = 0.0;
	aty = 0.0;
	atz = 0.0;
	world = aWorld;
	this->bIsAttached = false; // am I attached to an object now?
	glClearColor(0.0, 0.0 , 0.0, 0.0 );
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}
CameraControl::CameraControl(double ex, double ey, double ez, VNEWorld* aWorld)
{
	eyex = ex;
	eyey = ey;
	eyez = ez;
	atx = 0.0;
	aty = 0.0;
	atz = 0.0;
	initx = ex;
	inity = ey;
	initz = ez;
	world = aWorld;
	glClearColor(0.0, 0.0 , 0.0, 0.0 );
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}
void CameraControl::ResetPosition( )
{
	eyex = this->initx;
	eyey = this->inity;
	eyez = this->initz;
	this->Detach();
	ResizeCallbackHandler( this->iWindowW, this->iWindowH );
}
void CameraControl::UpdateAttachedCamera()
{
	if( this->bIsAttached )
		this->ResizeCallbackHandler(this->iWindowW, this->iWindowH);
}

void CameraControl::TranslateTo( double x, double y, double z )
{
	eyex = x;
	eyey = y;
	eyez = z;
	ResizeCallbackHandler( this->iWindowW, this->iWindowH );
}

void CameraControl::TranslateBy( double dx, double dy, double dz)
{
	this->eyex += dx;
	this->eyey += dy;
	this->eyez += dz;
	this->ResizeCallbackHandler(iWindowW,iWindowH);
}

void CameraControl::PointAt( double x, double y, double z )
{
	atx = x;
	aty = y;
	atz = z;
	ResizeCallbackHandler( this->iWindowW, this->iWindowH );
}

void CameraControl::ResizeCallbackHandler(int w, int h)
{
	iWindowW = w;
	iWindowH = h;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
}