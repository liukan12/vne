// define a vector valued force that will accelerate objects
#ifndef WORLDFORCE
#define WORLDFORCE

#include "VNEObject.h"

class WorldForce
{
private:
	double dScale; // bounciness of the bounding region 'walls'
	double dAtmDensity; // density of atmosphere
	double dWallReflectance;
	int iMode;
	bool bVortexOn;
	bool bAtmosphereOn;
	bool bWallsOn;
public:
	WorldForce( );
	WorldForce( string fileNameForceDef ); // need a format for defining force fields
	virtual void AccelerateObject( VNEObject* obj, double dTimeStep = 0.01);
	void VortexOn() {bVortexOn = true;}
	void VortexOff() {bVortexOn = false;}
	void AtmOn() {bAtmosphereOn = true;}
	void AtmOff() {bAtmosphereOn = false;}
	bool AtmIsOn() { return bAtmosphereOn; }
	void WallsOn() {bWallsOn = true; }
	void WallsOff() {bWallsOn = false; }
	bool WallsAreOn() { return bWallsOn; }
	double GetAtmDensity() { return dAtmDensity; } // later: draw thicker fog when atmosphere is thick

};

#endif