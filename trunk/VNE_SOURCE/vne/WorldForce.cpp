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

double mu = 20.0;

WorldForce::WorldForce( )
{
	this->iMode = 0; // various modes define different forces over the world
	this->dAtmDensity = 0.01;
	this->dWallReflectance = .9;
	this->bAtmosphereOn = true;
	this->bVortexOn = false;
	this->bWallsOn = true;
	this->bConvectionOn = true;

}

	

double WorldForce::UpdateForces( const valarray<double> &vertx, const valarray<double> &verty, const valarray<double> &vertz,
								valarray<double> &FX, valarray<double> &FY, valarray<double> &FZ,
								const valarray<double> &Velocity, const valarray<int> &VertControlPts,
								const int &numVerts, valarray<double> &offset, const double &dt)
{
	double bdry = 5.0;
	double minX = vertx[ VertControlPts[0] ];
	double minY = verty[ VertControlPts[1] ];
	double minZ = vertz[ VertControlPts[2] ];
	double maxX = vertx[ VertControlPts[3] ];
	double maxY = verty[ VertControlPts[4] ];
	double maxZ = vertz[ VertControlPts[5] ];
	double dOvershoot = 0.0;

	offset = 0.0; // correction for overlap with bounds
	frictionForce	= 0.0;
	gravForce		= 0.0;
	wallForce		= 0.0;
	windForce		= 0.0;
	dragForce		= 0.0;

	gravForce[1] = -50.0 / numVerts;
	dragForce = -dAtmDensity * Velocity;
	dragForce /= numVerts;

	frictionForce = -mu * Velocity / double( numVerts );

	if( bConvectionOn )
	{
		for( int i = 0; i < numVerts; i++ ) {
			if( vertx[i] > 0.0 )
				windForce[2] = -10.0;
			else
				windForce[2] = 10.0;
			if( vertz[i] > 0.0 )
				windForce[0] = 10.0;
			else
				windForce[0] = -10.0;
			
			windForce /= numVerts;
		}
	}

	if( bWallsOn )
	{
		if( minX < -bdry )	{
			offset[0] = -bdry - minX;
			dOvershoot = abs( abs(bdry) - abs(minX) );
			wallForce[0] = -(1.0 + dWallReflectance)*Velocity[0]/(dt);
			FX[ VertControlPts[0] ] += wallForce[0];
		}
		if( minY < -bdry )	{ // should NOT need abs(vel); the temporal resolution should take care of
			// getting the object collided and out of this zone in one step
			offset[1] = -bdry - minY;
			dOvershoot = abs( abs(bdry) - abs(minY) );
			wallForce[1] = -(1.0 + dWallReflectance)*Velocity[1]/(dt);
			FY[ VertControlPts[1] ] += wallForce[1] - gravForce[1]*numVerts;
			//FX += frictionForce[0];
			//FZ += frictionForce[2];
		}
		if( minZ < -bdry )	{
			offset[2] = -bdry - minZ;
			dOvershoot = abs( abs(bdry) - abs(minZ) ) ;
			wallForce[2] = -(1.0 + dWallReflectance)*Velocity[2]/(dt);
			FZ[ VertControlPts[2] ] += wallForce[2];
		}
		if( maxX > bdry )	{
			offset[0] = bdry - maxX;
			dOvershoot = abs( abs(bdry) - abs(maxX) );
			wallForce[0] = -(1.0 + dWallReflectance)*Velocity[0]/(dt);
			FX[ VertControlPts[3] ] += wallForce[0];
		}
		if( maxY > bdry )	{
			offset[1] = bdry - maxY;
			dOvershoot = abs( abs(bdry) - abs(maxY) );
			wallForce[1] = -(1.0 + dWallReflectance)*Velocity[1]/(dt);
			FY[ VertControlPts[4] ] += wallForce[1] - gravForce[1]*numVerts;
		}
		if( maxZ > bdry )	{
			offset[2] = bdry - maxZ;
			dOvershoot = abs( abs(bdry) - abs(maxZ) );
			wallForce[2] = -(1.0 + dWallReflectance)*Velocity[2]/(dt);
			FZ[ VertControlPts[5] ] += wallForce[2];
		}
	}
	for( int i = 0; i < numVerts; i++ )
	{
		FX[i] += dragForce[0] + windForce[0] + gravForce[0];
		FY[i] += dragForce[1] + windForce[1] + gravForce[1];
		FZ[i] += dragForce[2] + windForce[2] + gravForce[2];
	}

	return dOvershoot;

}
	

//void WorldForce::AccelerateObject( VNEObject* obj )
//{
//	frictionForce = 0.0;
//	gravForce = 0.0;
//	wallForce = 0.0;
//	windForce = 0.0;
//	gravForce[1] = -50.0 / numVerts;
//	dragForce = -dAtmDensity * Velocity;
//	dragForce /= numVerts;
//	
//	AngVel *= (1 - (dAtmDensity) );
//
//	AddForceAllVerts( gravForce );
//	AddForceAllVerts( dragForce );	
//	
//
//	if( bConvectionOn )
//	{
//		for( int i = 0; i < numVerts; i++ ) {
//			if( CurTriVertX[i] > 0.0 )
//				windForce[2] = -0.5;
//			else
//				windForce[2] = 0.5;
//			windForce /= numVerts;
//			AddForceAt( i, windForce );
//		}
//	}
//
//	if( bWallsOn )
//	{
//		double bdry = 5.0;
//		double dPenetrationTime = 0.0;
//		//gravForce[1] *= ( 1 - exp( -pow(abs(minY + 5.0),2) ) );
//		GetMinMaxVert();
//		if( minX < -bdry )
//		{
//			dPenetrationTime = abs( abs(bdry) - abs(minX) ) / abs(Velocity[0]);
//			TranslateBy( -dPenetrationTime * Velocity[0], -dPenetrationTime * Velocity[1], -dPenetrationTime * Velocity[2] );
//			Velocity[0] = dWallReflectance * abs( Velocity[0] );
//			TranslateBy( -bdry - minX, 0.0, 0.0 );
//			
//			// v = deltaX / deltaT
//			// deltaT = deltaX \ V
//			
//		}
//		if( minY < -bdry )
//		{
//			dPenetrationTime = abs( abs(bdry) - abs(minY) ) / abs(Velocity[1]);
//			TranslateBy( -dPenetrationTime * Velocity[0], -dPenetrationTime * Velocity[1], -dPenetrationTime * Velocity[2] );
//			Velocity[1] = dWallReflectance * abs( Velocity[1] );
//			GetMinMaxVert();
//			TranslateBy( 0.0, (TIMESTEP-dPenetrationTime)*Velocity[1], 0.0 );
//			GetMinMaxVert();
//			wallForce[1] = -gravForce[1] * numVerts;
//			AddForceAt( VertControlPts[1], wallForce );
//			
//		}
//		if( minZ < -bdry )
//		{
//			Velocity[2] = dWallReflectance * abs( Velocity[2] );
//			TranslateBy( 0.0, 0.0, -bdry - minZ );
//			
//		}
//		if( maxX > bdry )
//		{
//			Velocity[0] = -dWallReflectance * abs( Velocity[0] );
//			TranslateBy( bdry - maxX, 0.0, 0.0 );
//			
//		}
//		if( maxY > bdry )
//		{
//			Velocity[1] = -dWallReflectance * abs( Velocity[1] );
//			TranslateBy( 0.0, bdry - maxY, 0.0 );
//			
//		}
//		if( maxZ > bdry )
//		{
//			Velocity[2] = -dWallReflectance * abs( Velocity[2] );
//			TranslateBy( 0.0, 0.0, bdry - maxZ );
//			
//		}
//	}
//}

