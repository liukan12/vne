#ifndef VNEWORLD
#define VNEWORLD

#include "VNEObject.h"
#include "VNEObjList.h"
#include "WorldForce.h"
#include <time.h>
#include "VNETexture.h"

class VNEWorld
{
	// VNE World contains a list of objects
	// a global coordinate system
	// lighting (later)
	// global physical effects (later)
	// it interfaces with the drawing engine

private:
	
	VNETexture* myTex;
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double zmin;
	double zmax;
	double elapsedTime;
	bool bLightsOn;
	bool bTexturesOn;
	clock_t clock1;
	clock_t clock2;

public:
	VNEWorld();
	~VNEWorld();
	VNEObject* DemoObj; // a single demo object to start with
	VNEObjList* ObjList;   // later: a linked list of objects
	WorldForce* theForce;
	void TexturesOn();
	void TexturesOff();
	bool TexturesAreOn() { return bTexturesOn; }
	int TimeStep();
	int Redraw();
	void DrawWalls( );
	void EnableForce( int iNum );
	void DisableForce( int iNum );
	void LightsOn();
	void LightsOff();
	bool LightsAreOn() { return bLightsOn; }
	void PrintWorldState();

	double Getxmin();
	double Getymin();
	double Getzmin();
	double Getxmax();
	double Getymax();
	double Getzmax();
};



#endif