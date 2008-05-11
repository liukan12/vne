#include "VNEWorld.h"
//#include "VNEObject.h"

VNEWorld::VNEWorld()
{
	DemoObj = new VNEObject();
	xmin = -10.0;
	xmax = 10.0;
	ymin = -10.0;
	ymax = 10.0;
	zmin = -10.0;
	zmax = 10.0;
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