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
	string faces, faces2, faces3, faces4, faces5, faces6;
	string verts, verts2, verts3, verts4, verts5, verts6;
	string norms, norms2, norms3, norms4, norms5, norms6;
#define PROFILE_NO
#ifdef PROFILE
	faces =  "faces4.dat";
	verts =  "verts4.dat";
	norms =  "norms4.dat";
	faces2 = "faces5.dat";
	verts2 = "verts5.dat";
	norms2 = "norms5.dat";
	faces3 = "faces6.dat";
	verts3 = "verts6.dat";
	norms3 = "norms6.dat";
#elif _DEBUG // debug looks in relative path (vne data files)
	faces = "..\\vne_data\\faces4.dat";
	verts = "..\\vne_data\\verts4.dat";
	norms = "..\\vne_data\\norms4.dat";
	faces2 = "..\\vne_data\\faces5.dat";
	verts2 = "..\\vne_data\\verts5.dat";
	norms2 = "..\\vne_data\\norms5.dat";
	faces3 = "..\\vne_data\\faces6.dat";
	verts3 = "..\\vne_data\\verts6.dat";
	norms3 = "..\\vne_data\\norms6.dat";
	faces4 = "..\\vne_data\\faces7.dat";
	verts4 = "..\\vne_data\\verts7.dat";
	norms4 = "..\\vne_data\\norms7.dat";
	faces5 = "..\\vne_data\\faces8.dat";
	verts5 = "..\\vne_data\\verts8.dat";
	norms5 = "..\\vne_data\\norms8.dat";
	faces6 = "..\\vne_data\\faces9.dat";
	verts6 = "..\\vne_data\\verts9.dat";
	norms6 = "..\\vne_data\\norms9.dat";
#else // release build looks in the same directory as .exe for data files
	faces = "faces4.dat";
	verts = "verts4.dat";
	norms = "norms4.dat";
	faces2= "faces5.dat";
	verts2= "verts5.dat";
	norms2 = "norms5.dat";
	faces3= "faces6.dat";
	verts3= "verts6.dat";
	norms3= "norms6.dat";
	faces4 = "faces7.dat";
	verts4 = "verts7.dat";
	norms4 = "norms7.dat";
	faces5 = "faces8.dat";
	verts5 = "verts8.dat";
	norms5 = "norms8.dat";
	faces6 = "faces9.dat";
	verts6 = "verts9.dat";
	norms6 = "norms9.dat";
#endif

	// TODO: we need OBJECT FILES that define all file names and properties
	VNEObject* Obj1 = new VNEObject( "object 1", faces, verts, norms);
	VNEObject* Obj2 = new VNEObject( "object 2", faces2, verts2, norms2);
	VNEObject* Obj3 = new VNEObject( "object 3", faces3, verts3, norms3);
	VNEObject* Obj4 = new VNEObject( "object 4", faces4, verts4, norms4);
	VNEObject* Obj5 = new VNEObject( "object 5", faces5, verts5, norms5);
	VNEObject* Obj6 = new VNEObject( "object 6", faces6, verts6, norms6);
	VNEObject* Obj7 = new VNEObject( "object 7", faces4, verts4, norms4);
	VNEObject* Obj8 = new VNEObject( "object 8", faces5, verts5, norms5);
	VNEObject* Obj9 = new VNEObject( "object 9", faces6, verts6, norms6);
	Obj1->SetVelocityProfile( 0.5, 0.5, 0.0, 0 );
	Obj2->SetVelocityProfile( -0.5, 0.5, 0.5, 0 );
	Obj3->SetVelocityProfile( 0.5, -0.5, -0.5, 0 );
	Obj1->SetColorSeed(0.5,0.0,1.0);
	Obj2->SetColorSeed(1.0,0.0,0.5);
	Obj3->SetColorSeed(0.5,0.5,0.0);

	Obj4->TranslateTo( -5.0, -5.0, 2.0 );
	Obj5->TranslateTo( 5.0, -5.0, 0.0 );
	Obj6->TranslateTo( 5.0, 5.0, -3.0 );
	Obj7->TranslateTo( 0.0, -5.0, 2.0 );
	Obj8->TranslateTo( 0.0, 5.0, 0.0 );
	Obj9->TranslateTo( 5.0, 0.0, 0.0 );

	this->ObjList = new VNEObjList( Obj1 );
	this->ObjList->AddObj(Obj2);
	this->ObjList->AddObj(Obj3);
	this->ObjList->AddObj(Obj4);
	this->ObjList->AddObj(Obj5);
	this->ObjList->AddObj(Obj6);
	this->ObjList->AddObj(Obj7);
	this->ObjList->AddObj(Obj8);
	this->ObjList->AddObj(Obj9);

	this->ObjList->PrintAll();

	this->theForce = new WorldForce();
	theForce->VortexOff();

	xmin = -5;
	xmax = 5;
	ymin = -5;
	ymax = 5;
	zmin = -5;
	zmax = 15;
	clock1 = clock();
	elapsedTime = 0.0;
	glShadeModel( GL_SMOOTH );
	GLfloat light_position[] = {0.0,0.0,4.0,0.0};
	GLfloat light1_specular[] = {1.0,1.0,1.0,0.0};
	GLfloat light1_ambient[] = {1.0, 0.1, 0.5,0.0};
	GLfloat light1_diffuse[] = {1.0, 1.0, 1.0,0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse);
	GLfloat light2_position[] = {3.0,0.0,0.0,0.0};
	GLfloat light2_specular[] = {1.0,1.0,1.0,0.0};
	GLfloat light2_ambient[] = {0.5, 0.1, 1.0,0.0};
	GLfloat light2_diffuse[] = {1.0, 0.0, 1.0,0.0};
	glLightfv(GL_LIGHT1, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light2_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_diffuse);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
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

	result = this->ObjList->DrawAll();

	//glFlush();   // doesn't seem necessary

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
