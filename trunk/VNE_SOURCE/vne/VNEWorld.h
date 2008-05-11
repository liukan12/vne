#ifndef VNEWORLD
#define VNEWORLD

#include "VNEObject.h"

class VNEWorld
{
	// VNE World contains a list of objects
	// a global coordinate system
	// lighting (later)
	// global physical effects (later)
	// it interfaces with the drawing engine

private:
	VNEObject* DemoObj; // a single demo object to start with
	int AllocateVertexCoordsFromObj( );
	int AllocateVertexCoordsUniform(); // create the coordinate system
	double* coords; // [x0 y0 z0 x1 y1 z1 ... ]
	int coordSize; // number of entries in coords
	// VNEObjList* ObjList;   // later: a linked list of objects
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double zmin;
	double zmax;
	double space; // space between coordinates

public:
	VNEWorld();
	~VNEWorld();

	int TimeStep();
	int Redraw();

	double Getxmin();
	double Getymin();
	double Getzmin();
	double Getxmax();
	double Getymax();
	double Getzmax();
};



#endif