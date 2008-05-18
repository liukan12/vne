#ifndef VNEWORLD
#define VNEWORLD

#include "VNEObject.h"
#include "VNEObjList.h"
#include <time.h>

class VNEWorld
{
	// VNE World contains a list of objects
	// a global coordinate system
	// lighting (later)
	// global physical effects (later)
	// it interfaces with the drawing engine

private:
	
	
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double zmin;
	double zmax;
	double elapsedTime;
	clock_t clock1;
	clock_t clock2;

public:
	VNEWorld();
	~VNEWorld();
	VNEObject* DemoObj; // a single demo object to start with
	VNEObjList* ObjList;   // later: a linked list of objects
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