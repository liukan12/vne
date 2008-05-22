#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "WorldForce.h"
#include "VNEObject.h"

using namespace std;

WorldForce::WorldForce( )
{
	this->dScale = 10.0;
	iMode = 0; // various modes define different forces over the world
}


void WorldForce::AccelerateObject( VNEObject* obj, double dTimeStep )
{
	double fx, fy, fz;
	double cx, cy, cz;
	double sx, sy, sz;
	obj->GetCentroid(&cx, &cy, &cz);
	double mass = obj->GetMass();
	double dBdry = 4.0;
	switch( iMode )
	{
	case 0:
		
		fx = 0;
		fy = 0;
		fz = 0;
		
		// force gets bigger as you go outside the bounded region
		if( cx > dBdry )
			fx = -1.0*abs(cx)*abs(cx);
		if( cx < -dBdry)
			fx = 1.0*abs(cx)*abs(cx);
		if( cy > dBdry )
			fy = -1.0*abs(cy)*abs(cy);
		if( cy < -dBdry)
			fy = 1.0*abs(cy)*abs(cy);
		if( cz > dBdry )
			fz = -1.0*abs(cz)*abs(cz);
		if( cz < -dBdry)
			fz = 1.0*abs(cz)*abs(cz);
		fx = fx * dScale;
		fy = fy * dScale;
		fz = fz * dScale;
		obj->IncrementVelocity( fx*dTimeStep/mass, fy*dTimeStep/mass, fz*dTimeStep/mass );
		//break;
	case 1: // matrix with complex eigenvalues, negative real part -> spiral stable point at origin
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
		break;
	default:
		cout<<"Invalid Mode in WorldForce object acceleration\n";
	}


	
}

