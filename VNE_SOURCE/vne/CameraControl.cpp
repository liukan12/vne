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
#include "MathUtils.h"

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
	bIsAttached = false;
	glClearColor(0.0, 0.0 , 0.0, 0.0 );
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

void CameraControl::AttachToObject( VNEObject* obj )
{
	this->targetObj = obj;
	
	bIsAttached = true;
	UpdateAttachedCamera();
}

void CameraControl::ResetPosition( )
{
	eyex = this->initx;
	eyey = this->inity;
	eyez = this->initz;
	atx = 0.0;
	aty = 0.0;
	atz = 0.0;
	if( this->bIsAttached )
		this->Detach();
	ResizeCallbackHandler( this->iWindowW, this->iWindowH );
}
void CameraControl::UpdateAttachedCamera()
{
	if( this->bIsAttached )
	{
		double vx,vy,vz;
		this->targetObj->GetVelocity()->GetValueAt(&vx,&vy,&vz);
		double cx,cy,cz;
		this->targetObj->GetCentroid(&cx,&cy,&cz);
		double eyeOffset = 0.5;
		
		Normalize( &vx, &vy, &vz );
		this->eyex = cx + vx * eyeOffset;
		this->eyey = cy + vy * eyeOffset;
		this->eyez = cz + vz * eyeOffset;

		this->atx = cx + 2 * vx * eyeOffset;
		this->aty = cy + 2 * vy * eyeOffset;
		this->atz = cz + 2 * vz * eyeOffset;

		this->ResizeCallbackHandler(this->iWindowW, this->iWindowH);
	}
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