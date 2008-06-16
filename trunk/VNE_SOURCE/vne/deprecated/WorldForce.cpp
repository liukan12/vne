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
static valarray<double> compForce(3);
static valarray<double> dragTorque(3);
static valarray<double> windForce(3);
static valarray<double> frictionForce(3);
static valarray<double> temp3(3);
static valarray<double> temp3B(3);
valarray<double> RotVel(3);

double mu = 200.0;

WorldForce::WorldForce( )
{
	this->iMode = 0; // various modes define different forces over the world
	this->dAtmDensity = 0.01;
	this->dWallReflectance = .6;
	this->bAtmosphereOn = true;
	this->bVortexOn = false;
	this->bWallsOn = true;
	this->bConvectionOn = true;

}

void ComputeRotationalVelocity( const valarray<double> &AngVel, double x, double y, double z, const valarray<double> &Centroid )
{
		// temp3 represents the r vector, from the centroid to the action point of the force
		double rx = x - Centroid[0];
		double ry = y - Centroid[1];
		double rz = z - Centroid[2];

		// v = w cross r
		RotVel[0] = AngVel[1]*rz - AngVel[2]*ry;
		RotVel[1] = -AngVel[0]*rz + AngVel[2]*rx;
		RotVel[2] = AngVel[0]*ry - AngVel[1]*rx;
}

	

double WorldForce::UpdateForces( const valarray<double> &vertx, const valarray<double> &verty, const valarray<double> &vertz,
								valarray<double> &FX, valarray<double> &FY, valarray<double> &FZ,
								const valarray<double> &Velocity, const valarray<int> &VertControlPts,
								const int &numVerts, const valarray<double> &AngVel, 
								const valarray<double> &Centroid, const double &dt)
{
	double bdry = 5.0;
	double minX = vertx[ VertControlPts[0] ];
	double minY = verty[ VertControlPts[1] ];
	double minZ = vertz[ VertControlPts[2] ];
	double maxX = vertx[ VertControlPts[3] ];
	double maxY = verty[ VertControlPts[4] ];
	double maxZ = vertz[ VertControlPts[5] ];
	double dOvershoot = 0.0;

	gravForce		= 0.0;
	wallForce		= 0.0;
	windForce		= 0.0;
	dragForce		= 0.0;
	compForce		= 0.0;

	gravForce[1] = -50.0 / numVerts;
	dragForce = -dAtmDensity * Velocity;
	dragForce /= numVerts;

	frictionForce = -mu * Velocity;

	if( bConvectionOn )
	{
		for( int i = 0; i < numVerts; i++ ) {
			if( vertx[i] > 0.0 )
				FZ[i] += -20.0/numVerts;
			else
				FZ[i] += 20.0/numVerts;
			if( vertz[i] > 0.0 )
				FX[i] += 20.0 / numVerts;
			else
				FX[i] += -20.0/numVerts;
		}
		
	}

	if( bWallsOn )
	{
		if( minX < -bdry )	{
			ComputeRotationalVelocity( AngVel, vertx[VertControlPts[0]],verty[VertControlPts[0]],vertz[VertControlPts[0]],Centroid);
			dOvershoot = abs( abs(bdry) - abs(minX) );
			wallForce[0] = -(1.0 + dWallReflectance)*(RotVel[0]+Velocity[0])/(dt);
			compForce[0] = (1.0 + dWallReflectance) * (RotVel[0] / dt );
			FX[ VertControlPts[0] ] += wallForce[0];
		}
		if( minY < -bdry )	{ // should NOT need abs(vel); the temporal resolution should take care of
			// getting the object collided and out of this zone in one step
			ComputeRotationalVelocity( AngVel, vertx[VertControlPts[1]],verty[VertControlPts[1]],vertz[VertControlPts[1]],Centroid);
			dOvershoot = abs( abs(bdry) - abs(minY) );
			wallForce[1] = -(1.0 + dWallReflectance)*(RotVel[1]+Velocity[1])/(dt);
			compForce[1] = (1.0 + dWallReflectance) * (RotVel[1] / dt );
			FY[ VertControlPts[1] ] += wallForce[1] - gravForce[1]*numVerts;
			//FX[ VertControlPts[1] ] += frictionForce[0];
			//FZ[ VertControlPts[1] ] += frictionForce[2];
			//FX += -frictionForce[0] / numVerts;
			//FZ += -frictionForce[2] / numVerts;
		}
		if( minZ < -bdry )	{
			ComputeRotationalVelocity( AngVel, vertx[VertControlPts[2]],verty[VertControlPts[2]],vertz[VertControlPts[2]],Centroid);
			dOvershoot = abs( abs(bdry) - abs(minZ) ) ;
			wallForce[2] = -(1.0 + dWallReflectance)*(RotVel[2]+Velocity[2])/(dt);
			compForce[2] = (1.0 + dWallReflectance) * (RotVel[2] / dt );
			FZ[ VertControlPts[2] ] += wallForce[2];
		}
		if( maxX > bdry )	{
			ComputeRotationalVelocity( AngVel, vertx[VertControlPts[3]],verty[VertControlPts[3]],vertz[VertControlPts[3]],Centroid);
			dOvershoot = abs( abs(bdry) - abs(maxX) );
			wallForce[0] = -(1.0 + dWallReflectance)*(RotVel[0]+Velocity[0])/(dt);
			compForce[0] = (1.0 + dWallReflectance) * (RotVel[0] / dt );
			FX[ VertControlPts[3] ] += wallForce[0];
		}
		if( maxY > bdry )	{
			ComputeRotationalVelocity( AngVel, vertx[VertControlPts[4]],verty[VertControlPts[4]],vertz[VertControlPts[4]],Centroid);
			dOvershoot = abs( abs(bdry) - abs(maxY) );
			wallForce[1] = -(1.0 + dWallReflectance)*(RotVel[1]+Velocity[1])/(dt);
			compForce[1] = (1.0 + dWallReflectance) * (RotVel[1] / dt );
			FY[ VertControlPts[4] ] += wallForce[1] - gravForce[1]*numVerts;
		}
		if( maxZ > bdry )	{
			ComputeRotationalVelocity( AngVel, vertx[VertControlPts[5]],verty[VertControlPts[5]],vertz[VertControlPts[5]],Centroid);
			dOvershoot = abs( abs(bdry) - abs(maxZ) );
			wallForce[2] = -(1.0 + dWallReflectance)*(RotVel[2]+Velocity[2])/(dt);
			compForce[2] = (1.0 + dWallReflectance) * (RotVel[2] / dt );
			FZ[ VertControlPts[5] ] += wallForce[2];
		}
	}

	compForce /= double(numVerts);

	for( int i = 0; i < numVerts; i++ )
	{
		FX[i] += dragForce[0] + windForce[0] + gravForce[0] + compForce[0];
		FY[i] += dragForce[1] + windForce[1] + gravForce[1] + compForce[1];
		FZ[i] += dragForce[2] + windForce[2] + gravForce[2] + compForce[2];
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

