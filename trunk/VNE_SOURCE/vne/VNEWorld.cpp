#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "VNEApp.h"
#include "VNEWorld.h"

using std::cout;

VNEWorld::VNEWorld()
{
	DemoObj = new VNEObject();
	xmin = -5;
	xmax = 5;
	ymin = -5;
	ymax = 5;
	zmin = -5;
	zmax = 5;
	space = 0.1;
	//AllocateVertexCoordsUniform();
	AllocateVertexCoordsFromObj( );
}

int VNEWorld::AllocateVertexCoordsFromObj( )
{
	// each triangle face has three vertices, each of which have an x,y, and z component
	
	// Note: this shouldn't be necessary, just have each object allocate coordinates
	// on the fly...

	//coordSize = this->DemoObj->GetNumFaces() * 9;
	//coords = new double[coordSize];
	//this->DemoObj->GetCurTriVerts( coords, coordIdx );

	return 0;
}

int VNEWorld::AllocateVertexCoordsUniform()
{
	int result = 0;
	int i = 0;
	coordSize = 0;
	double xcurr = xmin;
	double ycurr = ymin;
	double zcurr = zmin;

	while( xcurr <= xmax )
	{
		while( ycurr <= ymax )
		{
			while( zcurr <= zmax )
			{
				coordSize++;
				zcurr += space;
			}
			zcurr = zmin;
			ycurr += space;
		}
		ycurr = ymin;
		xcurr += space;
	}

	xcurr = xmin;
	ycurr = ymin;
	zcurr = zmin;

	coords = new double[coordSize * 3];
	while( xcurr <= xmax )
	{
		while( ycurr <= ymax )
		{
			while( zcurr <= zmax )
			{
				this->coords[i] = xcurr;
				this->coords[i+1] = ycurr;
				this->coords[i+2] = zcurr;
				cout<<xcurr<<" "<<ycurr<<" "<<zcurr<<" \n";
				i += 3;
				zcurr += space;
			}
			zcurr = zmin;
			ycurr += space;
		}
		ycurr = ymin;
		xcurr += space;
	}

	return result;
}

int VNEWorld::TimeStep()
{
	int result = 0;

	// for now do nothing; later, following a script or physics sim
	// the objects will update position / rotation

	return result;
}

int VNEWorld::Redraw()
{
	int result = 0;
	
	this->DemoObj->DrawSelf();
	// one: we need the know what the vertices of the world coord system are
	// this can be done uniformly during initialization

	// alternate method: only allocate the ones that are in the object

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


//class VNEWorld
//{
//	// VNE World contains a list of objects
//	// a global coordinate system
//	// lighting (later)
//	// global physical effects (later)
//	// it interfaces with the drawing engine
//
//private:
//	VNEObject* DemoObj; // a single demo object to start with
//	// VNEObjList* ObjList;   // later: a linked list of objects
//	double xmin;
//	double xmax;
//	double ymin;
//	double ymax;
//	double zmin;
//	double zmax;
//
//public:
//	VNEWorld();
//	~VNEWorld();
//	double Getxmin();
//	double Getymin();
//	double Getzmin();
//	double Getxmax();
//	double Getymax();
//	double Getzmax();
//	
//
//};