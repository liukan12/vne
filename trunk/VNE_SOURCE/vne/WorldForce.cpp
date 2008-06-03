#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "WorldForce.h"
#include "VNEObject.h"

using namespace std;

static valarray<double> gravForce(3);
static valarray<double> wallForce(3);
static valarray<double> dragForce(3);
static valarray<double> dragTorque(3);
static valarray<double> windForce(3);
static valarray<double> frictionForce(3);
static valarray<double> temp3(3);
static valarray<double> temp3B(3);

double mu = 10.0;

WorldForce::WorldForce( )
{
	this->iMode = 0; // various modes define different forces over the world
	this->dAtmDensity = 0.001;
	this->dWallReflectance = .5;
	this->bAtmosphereOn = true;
	this->bVortexOn = false;
	this->bWallsOn = true;
	this->bConvectionOn = true;

}

void WorldForce::AccelerateObject( VNEObject* obj )
{
	frictionForce = 0.0;
	gravForce = 0.0;
	gravForce[1] = -0.1;
	dragForce = -dAtmDensity * obj->Velocity;
	
	obj->AngVel *= (1 - sqrt( dAtmDensity) );

	obj->AddForceAllVerts( gravForce );
	obj->AddForceAllVerts( dragForce );	
	

	if( bConvectionOn )
	{
		for( int i = 0; i < obj->numVerts; i++ ) {
			if( obj->CurTriVertX[i] > 0.0 )
				windForce[2] = -0.1;
			else
				windForce[2] = 1.0;
			obj->AddForceAt( i, windForce );
		}
	}

	if( bWallsOn )
	{
		double bdry = 5.0;
		gravForce[1] *= ( 1 - exp( -pow(abs(obj->minY + 5.0),2) ) );
		obj->GetMinMaxVert();
		if( obj->minX < -bdry )
		{
			obj->Velocity[0] = dWallReflectance * abs( obj->Velocity[0] );
			obj->TranslateBy( -bdry - obj->minX, 0.0, 0.0 );
			////frictionForce[1] = -mu * obj->Velocity[1];
			////frictionForce[2] = -mu * obj->Velocity[2];
			obj->AddForceAt( obj->VertControlPts[0], frictionForce );
		}
		if( obj->minY < -bdry )
		{
			obj->Velocity[1] = dWallReflectance * abs( obj->Velocity[1] );
			obj->TranslateBy( 0.0, -bdry - obj->minY, 0.0 );
			wallForce[1] = -gravForce[1] * obj->numVerts;
			obj->AddForceAt( obj->VertControlPts[1], wallForce );
			//frictionForce[0] = -mu * obj->Velocity[0];
			//frictionForce[2] = -mu * obj->Velocity[2];
			obj->AddForceAt( obj->VertControlPts[1], frictionForce );
		}
		if( obj->minZ < -bdry )
		{
			obj->Velocity[2] = dWallReflectance * abs( obj->Velocity[2] );
			obj->TranslateBy( 0.0, 0.0, -bdry - obj->minZ );
			//frictionForce[0] = -mu * obj->Velocity[0];
			//frictionForce[1] = -mu * obj->Velocity[1];
			obj->AddForceAt( obj->VertControlPts[2], frictionForce );
		}
		if( obj->maxX > bdry )
		{
			obj->Velocity[0] = -dWallReflectance * abs( obj->Velocity[0] );
			obj->TranslateBy( bdry - obj->maxX, 0.0, 0.0 );
			//frictionForce[1] = -mu * obj->Velocity[1];
			//frictionForce[2] = -mu * obj->Velocity[2];
			obj->AddForceAt( obj->VertControlPts[3], frictionForce );
		}
		if( obj->maxY > bdry )
		{
			obj->Velocity[1] = -dWallReflectance * abs( obj->Velocity[1] );
			obj->TranslateBy( 0.0, bdry - obj->maxY, 0.0 );
			//frictionForce[0] = -mu * obj->Velocity[0];
			//frictionForce[2] = -mu * obj->Velocity[2];
			obj->AddForceAt( obj->VertControlPts[4], frictionForce );
		}
		if( obj->maxZ > bdry )
		{
			obj->Velocity[2] = -dWallReflectance * abs( obj->Velocity[2] );
			obj->TranslateBy( 0.0, 0.0, bdry - obj->maxZ );
			//frictionForce[0] = -mu * obj->Velocity[0];
			//frictionForce[1] = -mu * obj->Velocity[1];
			obj->AddForceAt( obj->VertControlPts[5], frictionForce );
		}
	}



	
}

