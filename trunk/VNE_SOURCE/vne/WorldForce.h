// define a vector valued force that will accelerate objects
#ifndef WORLDFORCE
#define WORLDFORCE

#include "VNEObject.h"

class WorldForce
{
private:
	double dScale;
	int iMode;
public:
	WorldForce( );
	WorldForce( string fileNameForceDef ); // need a format for defining force fields
	virtual void AccelerateObject( VNEObject* obj, double dTimeStep = 0.01);

};

#endif