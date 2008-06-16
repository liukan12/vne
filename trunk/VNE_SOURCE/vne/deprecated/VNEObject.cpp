#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "VNEObject.h"
#include "IOUtils.h"
#include <valarray>
#include <numeric>

#define MANUAL_ROTATION // do I use rotation matrix code and keep track of precise vertex locations?
// this is crucial for high-accuracy physics, but slower...

using namespace std;

static valarray<double> temp3(3);
static valarray<double> temp3B(3);
static valarray<double> tempq(4);

// heap allocated matrix that is used for multiplication for rotations
// why here? => Don't create / destroy loads of matrices, just use ONE that hangs around!

VNEObject::VNEObject(const VNEObject& obj)
{
	CopyObj(obj);
}
VNEObject& VNEObject::operator=(const VNEObject& rhs)
{
	CopyObj(rhs);
	return *this;
}
void VNEObject::CopyObj(const VNEObject& obj)
{
	CurTriVertX = obj.CurTriVertX;
	CurTriVertY = obj.CurTriVertY;
	CurTriVertZ = obj.CurTriVertZ;
	CurTriNormX= obj.CurTriNormX;
	CurTriNormY= obj.CurTriNormY;
	CurTriNormZ= obj.CurTriNormZ;
	CurTriIdx = obj.CurTriIdx;
	Velocity = obj.Velocity;
	AngVel = obj.AngVel;
	Centroid = obj.Centroid;
	objTexture = obj.objTexture;

	rseed = obj.rseed;
	gseed = obj.gseed; 
	bseed = obj.bseed;
	colorVariance = obj.colorVariance;
	mass =obj.colorVariance;
	numFaces = obj.numFaces;
	objName =obj.objName;

	radSquared = obj.radSquared;
	elapsedTime = obj.elapsedTime;
	rotSpeed = obj.rotSpeed;

	bHasTexture = obj.bHasTexture;
	bIsStatic = obj.bIsStatic;
}
VNEObject::~VNEObject()
{
	
	delete objTexture;
}
int VNEObject::DrawSelf()
{
	int coordSize = this->numFaces * 9;
	int iRet = 0;
	int i;
	
	glPushMatrix();
	
	glTranslatef( this->Centroid[0], this->Centroid[1], this->Centroid[2] );

#ifndef MANUAL_ROTATION
	// using this method isn't even physically relevant so... only use it for show I guess
	glRotatef( angularVelocity * 0.1/mass + dCurrAngle, mx, my, mz );
	dCurrAngle = angularVelocity * 0.1/mass + dCurrAngle;
#endif
	glTranslatef( -Centroid[0], -Centroid[1], -Centroid[2] );
	if(this->bHasTexture)
	{	this->objTexture->bindTexture();
		for( i = 0; i < numFaces; i++ )
		{
			double offset = this->colorVariance*( i / double(numFaces) - 0.5 );
			glColor3f( rseed+offset, gseed+offset,bseed+offset);
			glBegin(GL_TRIANGLES);
				glNormal3d(CurTriNormX[ CurTriIdx[3*i+0] ], CurTriNormY[ CurTriIdx[3*i+0] ], CurTriNormZ[ CurTriIdx[3*i+0] ]);
				glTexCoord2f(0.0, 0.0);
				glVertex3d(CurTriVertX[ CurTriIdx[3*i+0] ], CurTriVertY[ CurTriIdx[3*i+0] ], CurTriVertZ[ CurTriIdx[3*i+0] ]);
				
				glNormal3d(CurTriNormX[ CurTriIdx[3*i+1] ], CurTriNormY[ CurTriIdx[3*i+1] ], CurTriNormZ[ CurTriIdx[3*i+1] ]);
				glTexCoord2f(1.0, 1.0);
				glVertex3d(CurTriVertX[ CurTriIdx[3*i+1] ], CurTriVertY[ CurTriIdx[3*i+1] ], CurTriVertZ[ CurTriIdx[3*i+1] ]);
				
				glNormal3d(CurTriNormX[ CurTriIdx[3*i+2] ], CurTriNormY[ CurTriIdx[3*i+2] ], CurTriNormZ[ CurTriIdx[3*i+2] ]);
				glTexCoord2f(0.0, 1.0);
				glVertex3d(CurTriVertX[ CurTriIdx[3*i+2] ], CurTriVertY[ CurTriIdx[3*i+2] ], CurTriVertZ[ CurTriIdx[3*i+2] ]);
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);

	}
	else
	{
		for( i = 0; i < numFaces; i++ )
		{

			double offset = this->colorVariance*( i / double(numFaces) - 0.5 );
			glColor3f( rseed+offset, gseed+offset,bseed+offset);
			glBegin(GL_TRIANGLES);
				glNormal3d(CurTriNormX[ CurTriIdx[3*i+0] ], CurTriNormY[ CurTriIdx[3*i+0] ], CurTriNormZ[ CurTriIdx[3*i+0] ]);
				glVertex3d(CurTriVertX[ CurTriIdx[3*i+0] ], CurTriVertY[ CurTriIdx[3*i+0] ], CurTriVertZ[ CurTriIdx[3*i+0] ]);
				glNormal3d(CurTriNormX[ CurTriIdx[3*i+1] ], CurTriNormY[ CurTriIdx[3*i+1] ], CurTriNormZ[ CurTriIdx[3*i+1] ]);
				glVertex3d(CurTriVertX[ CurTriIdx[3*i+1] ], CurTriVertY[ CurTriIdx[3*i+1] ], CurTriVertZ[ CurTriIdx[3*i+1] ]);
				glNormal3d(CurTriNormX[ CurTriIdx[3*i+2] ], CurTriNormY[ CurTriIdx[3*i+2] ], CurTriNormZ[ CurTriIdx[3*i+2] ]);
				glVertex3d(CurTriVertX[ CurTriIdx[3*i+2] ], CurTriVertY[ CurTriIdx[3*i+2] ], CurTriVertZ[ CurTriIdx[3*i+2] ]);
			glEnd();
		}
	}
	glPopMatrix();
	glFinish();

	return iRet;
}
void VNEObject::SetSpeed( double dSpeed )
{ 
	double dnorm = sqrt( Velocity[0]*Velocity[0] + Velocity[1]*Velocity[1] + Velocity[2]*Velocity[2] ); 
	
	if( dSpeed > 50.0 )
	{
		dSpeed = 50.0;
		//cout<<"Max Speed Reached!\n";
	}
	else if( dSpeed < 1e-6 )
		dSpeed = 1e-6;
	Velocity = Velocity / dnorm * dSpeed;
}

void VNEObject::SetRotSpeed( double dAngVel )
{
	temp3 = AngVel * AngVel;
	AngVel = dAngVel / sqrt(temp3.sum()) * AngVel;
	rotSpeed = dAngVel;
}

void VNEObject::PrintSelf()
{
#ifdef _DEBUG
	cout<<"Hi, I am VNEObject named: "<<this->objName<<" \n ";
#endif
}

void VNEObject::GetCentroid(double *dx, double* dy, double* dz)
{
	*dx = Centroid[0];
	*dy = Centroid[1];
	*dz = Centroid[2];
}

valarray<double> V;
valarray<double> VeX;
valarray<double> VeY;
valarray<double> CZ;
valarray<double> R;
valarray<double> Q;
valarray<double> C;
valarray<double> offset(3);
void VNEObject::IncrementTime()
{

	double dt = TIMESTEP;
	double spatialRes = TIMESTEP * MaxVelMag;
	elapsedTime += dt;
	
	this->ComputeTorqueDistribution( );
	this->UpdateVelocity( dt );
	this->UpdatePosition( dt );
	this->GetMinMaxVert( );
	 
	this->UpdateRotation( dt );	
	this->GetMinMaxVert( );
	this->ComputeInertia( ); // this is slow but necessary if we want to allow objects to deform in the future
	// if the body is not allowed to deform, you can get I directly by I = R * I_initial * R_transpose
	// where I_initial was computed once at initialization and R is the current rotation matrix
	ForceDistributionX = 0.0;
	ForceDistributionY = 0.0;
	ForceDistributionZ = 0.0;
	TorqueDistributionX = 0.0;
	TorqueDistributionY = 0.0;
	TorqueDistributionZ = 0.0;

	double overshoot = this->theForce->UpdateForces(CurTriVertX,CurTriVertY,CurTriVertZ,ForceDistributionX,ForceDistributionY,
	ForceDistributionZ, Velocity, VertControlPts,numVerts, AngVel, Centroid, dt);

	AngVel = AngVel * (1 - this->theForce->GetAtmDen() );
	temp3 = Velocity * Velocity;
	if( temp3.sum() > pow(MaxVelMag,2.0) ) // don't let him go so fast that spatial accuracy breaks down
		Velocity = Velocity / temp3.sum() * pow(MaxVelMag,2.0);
	

}

void VNEObject::ComputeRotationalVelocity( )
{
	// v = w x r
	for( int i = 0; i < numVerts; i++ )
	{
		// temp3 represents the r vector, from the centroid to the action point of the force
		temp3[0] = CurTriVertX[i] - Centroid[0];
		temp3[1] = CurTriVertY[i] - Centroid[1];
		temp3[2] = CurTriVertZ[i] - Centroid[2];

		// torque = r cross F
		RotVelX[i] = AngVel[1]*temp3[2] - AngVel[2]*temp3[1];
		RotVelY[i] = -AngVel[0]*temp3[2] + AngVel[2]*temp3[0];
		RotVelZ[i] = AngVel[0]*temp3[1] - AngVel[1]*temp3[0];
	}


}

// new and improved physics routines!
void VNEObject::UpdatePosition( double dt)
{
	//ComputeNetForce(); // redundant???
	// take two terms of the taylor series
	double dx = dt * ( Velocity[0] + 0.5 * dt * NetForce[0] / mass );
	double dy = dt * ( Velocity[1] + 0.5 * dt * NetForce[1] / mass );
	double dz = dt * ( Velocity[2] + 0.5 * dt * NetForce[2] / mass );

	this->CurTriVertX += dx;
	this->CurTriVertY += dy;
	this->CurTriVertZ += dz;

	ComputeCentroid();

}

void VNEObject::UpdateVelocity( double dt )
{
	ComputeNetForce(); 
	double dx = dt * NetForce[0] / mass;
	double dy = dt * NetForce[1] / mass;
	double dz = dt * NetForce[2] / mass;

	this->Velocity[0] += dx;
	this->Velocity[1] += dy;
	this->Velocity[2] += dz;

}
	
void VNEObject::UpdateRotation( double dt )
{
	// step 1: Get the total applied torque
	ComputeNetTorque();

	// step 2: dw/dt = I \ NetTorque(t)
	temp3B = dt * NetTorque;  // RHS
	temp3 = temp3B;  // initial guess
	for( int i = 0; i < 5; i++ )
	{ // cross fingers and hope 5 Gauss-Seidel iterations work :-)
		temp3[0] = ( temp3B[0] - InertiaTensor[1]*temp3[1] - InertiaTensor[2]*temp3[2] ) / InertiaTensor[0];
		temp3[1] = ( temp3B[1] - InertiaTensor[3]*temp3[0] - InertiaTensor[5]*temp3[2] ) / InertiaTensor[4];
		temp3[2] = ( temp3B[2] - InertiaTensor[6]*temp3[0] - InertiaTensor[7]*temp3[1] ) / InertiaTensor[8];
	}
	this->AngVel = AngVel + temp3;
	
	// step 3: find the new q
	// dq/dt = (1/2) * w X q   , X being quarternion multiplication operation
	// [0,w] X [q]
	tempq[0]	= 0*Quarternion[0] - (AngVel[0]*Quarternion[1] + AngVel[1]*Quarternion[2] + AngVel[2]*Quarternion[3] );
	tempq[1]	= AngVel[1]*Quarternion[3] - AngVel[2]*Quarternion[2];
	tempq[2]	= -AngVel[0]*Quarternion[3] + AngVel[2]*Quarternion[1];
	tempq[3]	= AngVel[0]*Quarternion[2] - AngVel[1]*Quarternion[1];

	tempq[1]	+= 0*Quarternion[1] + Quarternion[0]*AngVel[0];
	tempq[2]	+= 0*Quarternion[2] + Quarternion[0]*AngVel[1];
	tempq[3]	+= 0*Quarternion[3] + Quarternion[0]*AngVel[2];
	
	this->Quarternion = Quarternion + dt * 0.5 * tempq;
	tempq = Quarternion*Quarternion;
	Quarternion = Quarternion / sqrt(tempq.sum());

	// step 4: apply updated R
	RotateLocal();

	ComputeCentroid();
}

void VNEObject::RotateLocal( )
{
	double s = Quarternion[0];
	double x = Quarternion[1];
	double y = Quarternion[2];
	double z = Quarternion[3];
	
	RotationMatrix[0] = 1-2*y*y - 2*z*z;
	RotationMatrix[1] = 2*x*y-2*s*z;
	RotationMatrix[2] = 2*x*z+2*s*y;
	RotationMatrix[3] = 2*x*y+2*s*z;
	RotationMatrix[4] = 1-2*x*x-2*z*z;
	RotationMatrix[5] = 2*y*z-2*s*x;
	RotationMatrix[6] = 2*x*z-2*s*y;
	RotationMatrix[7] = 2*y*z+2*s*x;
	RotationMatrix[8] = 1-2*x*x-2*y*y;
	
	for( int i = 0; i < numVerts; i++ )
	{
		this->CurTriVertX[i] = RotationMatrix[0] * RefTriVertX[i] + RotationMatrix[1] * RefTriVertY[i] + RotationMatrix[2] * RefTriVertZ[i] + Centroid[0];
		this->CurTriVertY[i] = RotationMatrix[3] * RefTriVertX[i] + RotationMatrix[4] * RefTriVertY[i] + RotationMatrix[5] * RefTriVertZ[i] + Centroid[1];
		this->CurTriVertZ[i] = RotationMatrix[6] * RefTriVertX[i] + RotationMatrix[7] * RefTriVertY[i] + RotationMatrix[8] * RefTriVertZ[i] + Centroid[2];
	}
	this->ComputeCentroid();
	
}

void VNEObject::ComputeInertia()
{
	this->InertiaTensor = 0.0;
	
		// diagonal elements
		MyTemp = MassDistribution	 * ( LocTriVertY * LocTriVertY + LocTriVertZ * LocTriVertZ );
		InertiaTensor[0] = MyTemp.sum();
		MyTemp = MassDistribution	 * ( LocTriVertX * LocTriVertX + LocTriVertZ * LocTriVertZ );
		InertiaTensor[4] = MyTemp.sum();
		MyTemp = MassDistribution	 * ( LocTriVertY * LocTriVertY + LocTriVertX * LocTriVertX );
		InertiaTensor[8] = MyTemp.sum();

		// off-diagonal elements
		MyTemp = MassDistribution * LocTriVertY * LocTriVertX;
		InertiaTensor[1] = -MyTemp.sum();
		InertiaTensor[3] = InertiaTensor[1];
		MyTemp = MassDistribution * LocTriVertZ * LocTriVertX;
		InertiaTensor[2] = -MyTemp.sum();
		InertiaTensor[6] = InertiaTensor[2];
		MyTemp = MassDistribution * LocTriVertY * LocTriVertZ;
		InertiaTensor[5] = -MyTemp.sum();
		InertiaTensor[7] = InertiaTensor[5];
	
}

void VNEObject::SetVelocity( const valarray<double> &vel )
{
	Velocity = vel;
}

void VNEObject::ComputeCentroid()
{
	double cx = 0.0;
	double cy = 0.0;
	double cz = 0.0;

	cx = CurTriVertX.sum();
	cy = CurTriVertY.sum();
	cz = CurTriVertZ.sum();

	cx /= numVerts;
	cy /= numVerts;
	cz /= numVerts;

	Centroid[0] = cx;
	Centroid[1] = cy;
	Centroid[2] = cz;

	LocTriVertX = CurTriVertX - cx;
	LocTriVertY = CurTriVertY - cy;
	LocTriVertZ = CurTriVertZ - cz;
}

void VNEObject::ComputeTorqueDistribution( )
{

	for( int i = 0; i < numVerts; i++ )
	{
		// temp3 represents the r vector, from the centroid to the action point of the force
		temp3[0] = CurTriVertX[i] - Centroid[0];
		temp3[1] = CurTriVertY[i] - Centroid[1];
		temp3[2] = CurTriVertZ[i] - Centroid[2];

		// torque = r cross F
		this->TorqueDistributionX[i] += temp3[1]*ForceDistributionZ[i] - temp3[2]*ForceDistributionY[i];
		this->TorqueDistributionY[i] += -temp3[0]*ForceDistributionZ[i] + temp3[2]*ForceDistributionX[i];
		this->TorqueDistributionZ[i] += temp3[0]*ForceDistributionY[i] - temp3[1]*ForceDistributionX[i];
	}
}

void VNEObject::AddForceAt( int vertIdx, const valarray<double> &Force )
{
	// temp3 represents the r vector, from the centroid to the action point of the force
	temp3[0] = CurTriVertX[vertIdx] - Centroid[0];
	temp3[1] = CurTriVertY[vertIdx] - Centroid[1];
	temp3[2] = CurTriVertZ[vertIdx] - Centroid[2];

	// torque = r cross F
	this->TorqueDistributionX[vertIdx] += temp3[1]*Force[2] - temp3[2]*Force[1];
	this->TorqueDistributionY[vertIdx] += -temp3[0]*Force[2] + temp3[2]*Force[0];
	this->TorqueDistributionZ[vertIdx] += temp3[0]*Force[1] - temp3[1]*Force[0];

	this->ForceDistributionX[vertIdx] += Force[0];
	this->ForceDistributionY[vertIdx] += Force[1];
	this->ForceDistributionZ[vertIdx] += Force[2];
}

void VNEObject::AddForceAllVerts( const valarray<double> &Force )
{
	this->ForceDistributionX += Force[0];
	this->ForceDistributionY += Force[1]; 
	this->ForceDistributionZ += Force[2];
}

void VNEObject::ComputeNetForce()
{
	NetForce[0] = ForceDistributionX.sum();
	NetForce[1] = ForceDistributionY.sum();
	NetForce[2] = ForceDistributionZ.sum();
}

void VNEObject::ComputeNetTorque()
{
	NetTorque[0] = TorqueDistributionX.sum();
	NetTorque[1] = TorqueDistributionY.sum();
	NetTorque[2] = TorqueDistributionZ.sum();
}

void VNEObject::GetMinMaxVert(	double* minX, double* maxX, double* minY,
								double*  maxY, double* minZ, double* maxZ,
								int* minXi, int* maxXi, int* minYi, 
								int* maxYi, int* minZi, int* maxZi )
{
	*minX = 1e3;
	*minY = 1e3;
	*minZ = 1e3;
	*maxX = -1e3;
	*maxY = -1e3;
	*maxZ = -1e3;
	for( int i = 0; i < this->numVerts; i++ )
	{
		if( CurTriVertX[i] < *minX ) {
			*minX =  CurTriVertX[i];
			*minXi = i;
		}
		if( CurTriVertY[i] < *minY ) {
			*minY =  CurTriVertY[i];
			*minYi = i;
		}
		if( CurTriVertZ[i] < *minZ ) {
			*minZ =  CurTriVertZ[i];
			*minZi = i;
		}
		if( CurTriVertX[i] > *maxX ) {
			*maxX =  CurTriVertX[i];
			*maxXi = i;
		}
		if( CurTriVertY[i] > *maxY ) {
			*maxY =  CurTriVertY[i];
			*maxYi = i;
		}
		if( CurTriVertZ[i] > *maxZ ) {
			*maxZ =  CurTriVertZ[i];
			*maxZi = i;
		}
	}
}
void VNEObject::GetMinMaxVert()
{
	minX = 1e3;
	minY = 1e3;
	minZ = 1e3;
	maxX = -1e3;
	maxY = -1e3;
	maxZ = -1e3;
	for( int i = 0; i < this->numVerts; i++ )
	{
		if( CurTriVertX[i] < minX ) {
			minX =  CurTriVertX[i];
			VertControlPts[0] = i;
		}
		if( CurTriVertY[i] < minY ) {
			minY =  CurTriVertY[i];
			VertControlPts[1] = i;
		}
		if( CurTriVertZ[i] < minZ ) {
			minZ =  CurTriVertZ[i];
			VertControlPts[2] = i;
		}
		if( CurTriVertX[i] > maxX ) {
			maxX =  CurTriVertX[i];
			VertControlPts[3] = i;
		}
		if( CurTriVertY[i] > maxY ) {
			maxY =  CurTriVertY[i];
			VertControlPts[4] = i;
		}
		if( CurTriVertZ[i] > maxZ ) {
			maxZ =  CurTriVertZ[i];
			VertControlPts[5] = i;
		}
	}
}
	
int VNEObject::TranslateTo(double dx, double dy, double dz)
{ // warning: this probably breaks now except at object initialization
	int i;
	for( i = 0; i < this->numVerts; i++ )
	{
		this->CurTriVertX[i] = dx - Centroid[0] + CurTriVertX[i];
		this->CurTriVertY[i] = dy - Centroid[1] + CurTriVertY[i];
		this->CurTriVertZ[i] = dz - Centroid[2] + CurTriVertZ[i];
	}

	ComputeCentroid();
	return 0;
}

int VNEObject::TranslateBy(double dx, double dy, double dz)
{
	int i;
	for( i = 0 ; i < this->numVerts ; i++ )
	{
		CurTriVertX[i] += dx;
		CurTriVertY[i] += dy;
		CurTriVertZ[i] += dz;
	}

	ComputeCentroid();
	return 0;
	
}

void VNEObject::ApplyInstTorque( const valarray<double> &Torque )
{
	TorqueDistributionX += Torque[0];
	TorqueDistributionY += Torque[1];
	TorqueDistributionZ += Torque[2];

}

void VNEObject::SetVelocityProfile(double xval, double yval, double zval, int iCode )
{
	
		this->Velocity[0] = xval;
		this->Velocity[1] = yval;
		this->Velocity[2] = zval;
}

double VNEObject::GetSpeed( )
{
	
	return sqrt( Velocity[0]*Velocity[0] + Velocity[1]*Velocity[1] + Velocity[2]*Velocity[2] );
}

double VNEObject::GetAngSpeed( )
{
	
	return sqrt( AngVel[0]*AngVel[0] + AngVel[1]*AngVel[1] + AngVel[2]*AngVel[2] );
}

void VNEObject::Scale( double ratiox, double ratioy, double ratioz )
{
	CurTriVertX *= ratiox;
	CurTriVertY *= ratioy;
	CurTriVertZ *= ratioz;
	RefTriVertX *= ratiox;
	RefTriVertY *= ratioy;
	RefTriVertZ *= ratioz;
	ComputeCentroid( );
	ComputeInertia( );
}

VNEObject::VNEObject( string objName, string fileNameFaces, string fileNameVerts, string fileNameNorms )
{
	this->objName = objName;

	elapsedTime = 0.0;
	mass = 1.0;

	colorVariance = 0.3; // how much will colors vary from the seed
	rseed = 1.0;
	gseed = 1.0;
	bseed = 1.0;

	this->rotSpeed = 0.001;
	
	this->Velocity = valarray<double>(3);
	this->AngVel = valarray<double>(3);
	this->Centroid = valarray<double>(3);
	this->NetForce = valarray<double>(3);
	this->NetTorque = valarray<double>(3);
	this->Quarternion = valarray<double>(4);
	this->numFaces = 0; // default before data read

	AngVel[0] = 0.0;
	AngVel[1] = 1.0;
	AngVel[2] = 0.0;

	Quarternion[0] = 1.0;

	cout<<"Reading mesh data...\n";

	numFaces = ReadMeshData( CurTriIdx, CurTriVertX, CurTriVertY, CurTriVertZ,
							 CurTriNormX, CurTriNormY, CurTriNormZ, 
							 fileNameFaces, fileNameVerts, fileNameNorms );

	numVerts = CurTriVertX.size();
	this->InertiaTensor = valarray<double>(9); // I, the 3x3 inertia tensor
	this->RotationMatrix = valarray<double>(9); // I, the 3x3 rotation matrix; rotates reference verts to current verts
	this->MassDistribution = valarray<double>(numVerts); // how much mass at each vertex is there
	this->MyTemp = valarray<double>(numVerts); // temp var for inertia computations
	
	this->LocTriVertX = valarray<double>(numVerts);
	this->LocTriVertY = valarray<double>(numVerts);
	this->LocTriVertZ = valarray<double>(numVerts);

	this->ForceDistributionX = valarray<double>(numVerts);
	this->ForceDistributionY = valarray<double>(numVerts);
	this->ForceDistributionZ = valarray<double>(numVerts);
	this->TorqueDistributionX = valarray<double>(numVerts);
	this->TorqueDistributionY = valarray<double>(numVerts);
	this->TorqueDistributionZ = valarray<double>(numVerts);

	this->VertControlPts = valarray<int>(6);
	this->GetMinMaxVert();
	this->theForce = new WorldForce();

	MassDistribution = mass / (numVerts); // uniform mass distribution

	ComputeCentroid();
	ComputeInertia();	

	this->RefTriVertX = CurTriVertX - Centroid[0];
	this->RefTriVertY = CurTriVertY - Centroid[1];
	this->RefTriVertZ = CurTriVertZ - Centroid[2];

	RotateLocal();

	this->bHasTexture=false;
}
void VNEObject::setTexture(string fileName)
{
	this->objTexture=new VNETexture(fileName);
	this->bHasTexture=true;
}

void VNEObject::SetTimeStep( double dt, double vmax )
{
	TIMESTEP = dt;
	MaxVelMag = vmax;
}

void VNEObject::SetColorSeed( double r, double g, double b )
{
	//this->ColorSeed->SetValues(r,g,b);
	rseed = r;
	gseed = g;
	bseed = b;
}

void VNEObject::GetColorSeed( double* r, double* g, double* b)
{
	//this->ColorSeed->GetValueAt(r,g,b);
	*r = rseed;
	*b = bseed;
	*g = gseed;
}

VNEObject::VNEObject()
{ // this is bogus	

}