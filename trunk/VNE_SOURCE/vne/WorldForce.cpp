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
	this->dAtmDensity = 0.001;
	this->dWallReflectance = 0.999;
	this->bAtmosphereOn = true;
	this->bVortexOn = false;
	this->bWallsOn = true;

}

void ConvectionForce( valarray<double> &position, valarray<double> &force )
{
	// given x,y,z position, what is the convective force there?
	force[0] = abs(position[0]) > 0.5 ? -0.05*position[0] : 0.0;
	force[1] = abs(position[1]) > 0.5 ? -0.05*position[1] : 0.0;
	force[2] = abs(position[2]) > 0.5 ? -0.05*position[2] : 0.0;
}


void WorldForce::AccelerateObject( VNEObject* obj, double dTimeStep )
{
	double fx, fy, fz;
	double cx, cy, cz;
	double sx, sy, sz;
	
	cx = obj->GetCentroid()[0];
	cy = obj->GetCentroid()[1];
	cz = obj->GetCentroid()[2];
	sx = obj->GetVelocity()[0];
	sy = obj->GetVelocity()[1];
	sz = obj->GetVelocity()[2];

	double mass = obj->GetMass();
	double dBdry = 4.0;
	
			
	if( bWallsOn )
	{
		// force gets bigger as you go outside the bounded region
		if( cx > dBdry )
			sx = -dWallReflectance*abs(sx);
		if( cx < -dBdry)
			sx = dWallReflectance*abs(sx);
		if( cy > dBdry )
			sy = -dWallReflectance*abs(sy);
		if( cy < -dBdry)
			sy = dWallReflectance*abs(sy);
		if( cz > dBdry )
			sz = -dWallReflectance*abs(sz);
		if( cz < -dBdry)
			sz = dWallReflectance*abs(sz);
		
		obj->SetVelocityProfile( sx, sy, sz, 0 );

		// TODO: how does impact affect the spin of the object??

	}
	if( bVortexOn )
	{
		
	}
	if( bAtmosphereOn )
	{
		sx = obj->GetVelocity()[0];
		sy = obj->GetVelocity()[1];
		sz = obj->GetVelocity()[2];
		obj->IncrementVelocity( -sx*dAtmDensity,-sy*dAtmDensity,-sz*dAtmDensity);
		double domega = obj->GetAngVelMag();
		obj->IncrementAngVel( -domega*dAtmDensity );
	}
	if( bConvectionOn )
	{
		ConvectionForce( obj->GetCentroid(), theForce );
		obj->IncrementVelocity( theForce[0], theForce[1], theForce[2] );
	}
	
}

