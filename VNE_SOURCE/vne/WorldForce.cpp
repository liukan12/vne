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
	obj->GetCentroid(&cx, &cy, &cz);
	double mass = obj->GetMass();

	switch( iMode )
	{
	case 0:
		
		fx = 0;
		fy = 0;
		fz = 0;
		
		if( cx > 2 )
			fx = -1.0;
		if( cx < -2)
			fx = 1.0;
		if( cy > 2 )
			fy = -1.0;
		if( cy < -2)
			fy = 1.0;
		if( cz > 2 )
			fz = -1.0;
		if( cz < -2)
			fz = 1.0;
		fx = fx * dScale;
		fy = fy * dScale;
		fz = fz * dScale;
		obj->IncrementVelocity( fx*dTimeStep/mass, fy*dTimeStep/mass, fz*dTimeStep/mass );
		break;
	default:
		cout<<"Invalid Mode in WorldForce object acceleration\n";
	}
	
}