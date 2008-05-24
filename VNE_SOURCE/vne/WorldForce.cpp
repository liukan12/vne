#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "WorldForce.h"
#include "VNEObject.h"

using namespace std;

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


void WorldForce::AccelerateObject( VNEObject* obj, double dTimeStep )
{
	double fx, fy, fz;
	double cx, cy, cz;
	double sx, sy, sz;
	obj->GetCentroid(&cx, &cy, &cz);
	double mass = obj->GetMass();
	double dBdry = 4.0;
	obj->GetVelocity()->GetValueAt(&sx, &sy, &sz );
			
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
	}
		// matrix with complex eigenvalues, negative real part -> spiral stable point at origin
		//(x') =  [-.01    -1] [x]
		//(y') =  [ 1    -.01] [y]
		// lambda = -.01 +/- sqrt(-1)
		if( bVortexOn )
		{
			//if( obj->GetSpeed() > 50.0 )
			
			obj->GetVelocity()->GetValueAt(&sx, &sy, &sz );
			fx = (-.1*cx-1*cy);
			fy = (1*cx-.1*cy);
			sx = fx - sx;//*dTimeStep/mass;
			sy = fy - sy;//*dTimeStep/mass;
			obj->IncrementVelocity( sx, sy, 0.0);
			obj->GetVelocity()->GetValueAt(&sx, &sy, &sz );
			fz = (-.3*cz-1*cy);
			fy = (1*cz-.3*cy);
			sz = fz - sz;//*dTimeStep/mass;
			sy = fy - sy;//*dTimeStep/mass;
			obj->IncrementVelocity( 0.0, sy, sz);
			obj->SetSpeed( 10.0*(cx*cx+cy*cy+cz*cz) );
		}
		if( bAtmosphereOn )
		{
			obj->GetVelocity()->GetValueAt(&sx, &sy, &sz );
			obj->IncrementVelocity( -sx*dAtmDensity,-sy*dAtmDensity,-sz*dAtmDensity);
			double domega = obj->GetAngVel();
			obj->IncrementAngVel( -domega*dAtmDensity );
		}
	
}

