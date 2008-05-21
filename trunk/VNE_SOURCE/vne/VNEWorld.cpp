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
#include "WorldForce.h"
using std::cout;

VNEWorld::VNEWorld()
{
	//DemoObj = new VNEObject();
	
	string faces, faces2, faces3;
	string verts, verts2, verts3;
#ifdef _DEBUG // debug looks in relative path (vne data files)
	faces = "..\\vne_data\\faces1.dat";
	verts = "..\\vne_data\\verts1.dat";
	faces2 = "..\\vne_data\\faces2A.dat";
	verts2 = "..\\vne_data\\verts2A.dat";
	faces3 = "..\\vne_data\\faces3A.dat";
	verts3 = "..\\vne_data\\verts3A.dat";
#else // release build looks in the same directory as .exe for data files
	faces = "faces1.dat";
	verts = "verts1.dat";
	faces2= "faces2A.dat";
	verts2= "verts2A.dat";
	faces3= "faces3A.dat";
	verts3= "verts3A.dat";
#endif


	VNEObject* Obj1 = new VNEObject( "object 1", faces, verts);
	VNEObject* Obj2 = new VNEObject( "object 2", faces2, verts2);
	VNEObject* Obj3 = new VNEObject( "object 3", faces3, verts3);
	Obj1->SetVelocityProfile( 0.5, 0.5, 0.0, 0 );
	Obj2->SetVelocityProfile( -0.5, 0.5, 0.5, 0 );
	Obj3->SetVelocityProfile( 0.5, -0.5, -0.5, 0 );
	
	this->ObjList = new VNEObjList( Obj1 );
	this->ObjList->AddObj(Obj2);
	this->ObjList->AddObj(Obj3);

	this->ObjList->PrintAll();

	this->theForce = new WorldForce();
	theForce->VortexOff();

	xmin = -5;
	xmax = 5;
	ymin = -5;
	ymax = 5;
	zmin = -5;
	zmax = 10;
	clock1 = clock();
	elapsedTime = 0.0;
	glShadeModel( GL_SMOOTH );
}

void VNEWorld::EnableForce( int iNum )
{
	switch( iNum )
	{
	case 0:
		this->theForce->VortexOn();
		break;
	default:
		cout<<"Invalid number in EnableForce()!\n";
		break;
	}
}
void VNEWorld::DisableForce( int iNum )
{
	switch( iNum )
	{
	case 0:
		this->theForce->VortexOff();
	default:
		cout<<"Invalid number in DisableForce()!\n";
		break;
	}
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

	//DemoObj->IncrementTime(); // use constant time flow, otherwise user interaction
	// causes errors (too much delay)
	this->ObjList->AccelAll( this->theForce );
	result = this->ObjList->TimeStepAll();

	return result;
}

int VNEWorld::Redraw()
{
	int result = 0;
	
	//this->DemoObj->DrawSelf();
	// one: we need the know what the vertices of the world coord system are
	// this can be done uniformly during initialization

	result = this->ObjList->DrawAll();

	glFlush();

	return result;
}

void VNEWorld::PrintWorldState()
{
	// output in console the state of all objects, forces, lighting, etc
	cout<<"The state of the world is: \n";

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
