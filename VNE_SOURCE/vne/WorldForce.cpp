#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "WorldForce.h"
#include "VNEObject.h"

using namespace std;

static valarray<double> theForce(3);

WorldForce::WorldForce( )
{
	this->dScale = .1;
	this->iMode = 0; // various modes define different forces over the world
	this->dAtmDensity = 0.01;
	this->dWallReflectance = 0.95;
	this->bAtmosphereOn = true;
	this->bVortexOn = false;
	this->bWallsOn = true;
	this->bConvectionOn = true;

}

void ConvectionForce( const valarray<double> &position, valarray<double> &force )
{
	// given x,y,z position, what is the convective force there?
	force[0] = 0*abs(position[0]) > 0.5 ? -0.05*position[0] : 0.0;
	force[1] = ( position[1] > -4.0 )? -0.1 : 0.0;
	force[2] = 0.0; //0*abs(position[2]) > 0.5 ? -0.05*position[2] : 0.0;
}


void WorldForce::AccelerateObject( VNEObject* obj, double dTimeStep )
{
		
	double dBdry = 4.0;
	
	double minX, maxX, minY, maxY, minZ, maxZ;
	int minXi, maxXi, minYi, maxYi, minZi, maxZi;

	obj->GetMinMaxVert( &minX, &maxX, &minY, &maxY, &minZ, &maxZ,
						&minXi, &maxXi, &minYi, &maxYi, &minZi, &maxZi );
			
	if( bWallsOn )
	{
		// force gets bigger as you go outside the bounded region
		if( obj->Centroid[0] > dBdry )
			obj->Velocity[0] = -dWallReflectance*abs(obj->Velocity[0]);
		if( obj->Centroid[0] < -dBdry)
			obj->Velocity[0] = dWallReflectance*abs(obj->Velocity[0]);
		if( obj->Centroid[1] > dBdry )
			obj->Velocity[1] = -dWallReflectance*abs(obj->Velocity[1]);
		if( obj->Centroid[1] < -dBdry)
			obj->Velocity[1] = dWallReflectance*abs(obj->Velocity[1]);
		if( obj->Centroid[2] > dBdry )
			obj->Velocity[2] = -dWallReflectance*abs(obj->Velocity[2]);
		if( obj->Centroid[2] < -dBdry)
			obj->Velocity[2] = dWallReflectance*abs(obj->Velocity[2]);

		// TODO: how does impact affect the spin of the object??

	}
	if( bVortexOn )
	{
		
	}
	if( bAtmosphereOn )
	{
		obj->IncrementVelocity( -dAtmDensity*(obj->Velocity) );
		double domega = obj->GetAngVelMag();
		obj->IncrementAngVel( -domega*dAtmDensity*0.1 );
	}
	if( bConvectionOn )
	{
		ConvectionForce( obj->Centroid, theForce );
		obj->IncrementVelocity( theForce );
	}
	
}

