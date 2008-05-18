#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "VNEApp.h"
#include "VNEWorld.h"
#include <time.h>

using std::cout;

VNEWorld::VNEWorld()
{
	DemoObj = new VNEObject();
	
	VNEObject* Obj1 = new VNEObject( "object 1" );
	this->ObjList = new VNEObjList( Obj1 );
	
	this->ObjList->PrintAll();

	VNEObject* Obj2 = new VNEObject( "object 2" );
	this->ObjList->AddObj( Obj2 );

	this->ObjList->PrintAll();

	xmin = -5;
	xmax = 5;
	ymin = -5;
	ymax = 5;
	zmin = -5;
	zmax = 5;
	clock1 = clock();
	elapsedTime = 0.0;
	glShadeModel( GL_SMOOTH );
}

int VNEWorld::TimeStep()
{
	int result = 0;

	// for now do nothing; later, following a script or physics sim
	// the objects will update position / rotation

	clock2 = clock();
	double dt = ( (double) (clock2 - clock1) ) / CLOCKS_PER_SEC ;
	elapsedTime += dt;
	clock1 = clock();

	DemoObj->IncrementTime(); // use constant time flow, otherwise user interaction
	// causes errors (too much delay)

	//result = this->ObjList->TimeStepAll();

	return result;
}

int VNEWorld::Redraw()
{
	int result = 0;
	
	this->DemoObj->DrawSelf();
	// one: we need the know what the vertices of the world coord system are
	// this can be done uniformly during initialization

	result = this->ObjList->DrawAll();

	glFlush();

	return result;
}

double VNEWorld::Getxmax()
{ return xmax; }

double VNEWorld::Getymax()
{ return ymax; }

double VNEWorld::Getzmax()
{ return zmax; }

double VNEWorld::Getxmin()
{ return xmin; }

double VNEWorld::Getymin()
{ return ymin; }

double VNEWorld::Getzmin()
{ return zmin; }
