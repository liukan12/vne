#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "WorldForce.h"
#include "VNEObject.h"

using namespace std;

static valarray<double> gravForce(3);
static valarray<double> WallForce(3);
static valarray<double> temp3(3);
static valarray<double> temp3B(3);

WorldForce::WorldForce( )
{
	this->dScale = .1;
	this->iMode = 0; // various modes define different forces over the world
	this->dAtmDensity = 0.001;
	this->dWallReflectance = .95;
	this->bAtmosphereOn = true;
	this->bVortexOn = false;
	this->bWallsOn = true;
	this->bConvectionOn = true;

}

void GravForce( const valarray<double> &position, valarray<double> &force )
{
	// given x,y,z position, what is the convective force there?
	force[0] = 0.0;
	force[1] = ( position[1] > -4.001 )? -5.0: 0.0;
	force[2] = 0*( position[0] > 0.0 )? -1.0: 1.0;;
}

void GravForce( double x, double y, double z, valarray<double> &force )
{
	// given x,y,z position, what is the convective force there?
	force[0] = 0.0;
	force[1] = ( y > -4.001 )? -5.0: 0.0;
	force[2] = 0*( x > 0.0 )? -1.0: 1.0;;
}
void GravForce2( const valarray<double> &position, valarray<double> &force )
{
	// given x,y,z position, what is the convective force there?
	force[0] = 0.0;
	force[1] = 0*( position[1] > -4.001 )? -5.0: 0.0;
	force[2] = ( position[0] > 0.0 )? -1.0: 1.0;;
}

void GravForce2( double x, double y, double z, valarray<double> &force )
{
	// given x,y,z position, what is the convective force there?
	force[0] = 0.0;
	force[1] = 0*( y > -4.001 )? -5.0: 0.0;
	force[2] = ( x > 0.0 )? -1.0: 1.0;;
}

void WorldForce::AccelerateObject( VNEObject* obj, double dTimeStep )
{
		
	double dBdry = 5.0;
	
	double minX, maxX, minY, maxY, minZ, maxZ;
	int minXi, maxXi, minYi, maxYi, minZi, maxZi;

	obj->GetMinMaxVert( &minX, &maxX, &minY, &maxY, &minZ, &maxZ,
						&minXi, &maxXi, &minYi, &maxYi, &minZi, &maxZi );
			
	if( bWallsOn )
	{
		WallForce = 0;
		// force gets bigger as you go outside the bounded region
		if( maxX > dBdry ) {
			WallForce[0] = -1.0;
			obj->ForceWAt( maxXi, WallForce );
			obj->AngVel *= dWallReflectance;
			obj->rotSpeed *= dWallReflectance;
			obj->Velocity[0] = -dWallReflectance*abs(obj->Velocity[0]);
		}
		if( minX < -dBdry) {
			WallForce[0] = 1.0;
			obj->ForceWAt( minXi, WallForce );
			obj->AngVel *= dWallReflectance;
			obj->rotSpeed *= dWallReflectance;
			obj->Velocity[0] = dWallReflectance*abs(obj->Velocity[0]);
		}
		if( maxY > dBdry ) {
			WallForce[1] = -1.0;
			obj->ForceWAt( maxYi, WallForce );
			obj->AngVel *= dWallReflectance;
			obj->rotSpeed *= dWallReflectance;
			obj->Velocity[1] = -dWallReflectance*abs(obj->Velocity[1]);
		}
		if( minY < -dBdry) {
			WallForce[1] = 1.0;
			obj->ForceWAt( minYi, WallForce );
			obj->AngVel *= dWallReflectance;
			obj->rotSpeed *= dWallReflectance;
			obj->Velocity[1] = dWallReflectance*abs(obj->Velocity[1]);
		}
		if( maxZ > dBdry ) {
			WallForce[2] = -1.0;
			obj->ForceWAt( maxZi, WallForce );
			obj->AngVel *= dWallReflectance;
			obj->rotSpeed *= dWallReflectance;
			obj->Velocity[2] = -dWallReflectance*abs(obj->Velocity[2]);
		}
		if( minZ < -dBdry) {
			WallForce[2] = 1.0;
			obj->ForceWAt( minZi, WallForce );
			obj->AngVel *= dWallReflectance;
			obj->rotSpeed *= dWallReflectance;
			obj->Velocity[2] = dWallReflectance*abs(obj->Velocity[2]);
		}

	}
	if( bVortexOn )
	{
		
	}
	if( bAtmosphereOn )
	{
		obj->IncrementVelocity( -dAtmDensity*(obj->Velocity) );
		obj->SetRotSpeed( obj->GetAngVelMag() * (1 - dAtmDensity) );
		//double domega = obj->GetAngVelMag();
		//obj->IncrementAngVel( -domega*dAtmDensity*0.1 );
	}
	if( bConvectionOn )
	{
		temp3[0] = obj->CurTriVertX[minYi];
		temp3[1] = obj->CurTriVertY[minYi];
		temp3[2] = obj->CurTriVertZ[minYi];
		//GravForce( temp3, gravForce );
		obj->ComputeInertia();
		
		//obj->ApplyForceAllVerts( gravForce );
		
		temp3B = 0.0;
		for( int i = 0; i < obj->numVerts; i++ )
		{
			GravForce2( obj->CurTriVertX[i], obj->CurTriVertY[i], obj->CurTriVertZ[i], gravForce );
			// temp3 represents the r vector, from the centroid to the action point of the force
			temp3[0] = obj->CurTriVertX[i] - obj->Centroid[0];
			temp3[1] = obj->CurTriVertY[i] - obj->Centroid[1];
			temp3[2] = obj->CurTriVertZ[i] - obj->Centroid[2];

			// torque = r cross F
			temp3B[0] += temp3[1]*gravForce[2] - temp3[2]*gravForce[1];
			temp3B[1] += -temp3[0]*gravForce[2] + temp3[2]*gravForce[0];
			temp3B[2] += temp3[0]*gravForce[1] - temp3[1]*gravForce[0];
			
		}
		obj->IncrementVelocity( gravForce / obj->mass * TIMESTEP );
		obj->ApplyInstTorque( temp3B );
		GravForce( obj->Centroid, gravForce );
		obj->IncrementVelocity( gravForce / obj->mass * TIMESTEP );
		
		
			
	
	}
	
}

