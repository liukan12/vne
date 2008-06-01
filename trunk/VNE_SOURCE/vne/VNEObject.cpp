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

static valarray<double> RotationMatrix(9);
static valarray<double> temp3(3);
static valarray<double> temp3B(3);

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

valarray<double>* VNEObject::GetCentroid(){
	return &Centroid;
}

void VNEObject::IncrementTime()
{
	double dt = TIMESTEP;
	elapsedTime += dt;
		
	this->TranslateBy( Velocity[0]*dt/mass, Velocity[1]*dt/mass, Velocity[2]*dt/mass );

#ifdef MANUAL_ROTATION
	// this still needs to happen potentially if we want highly accurate collision handling
	// with angular momentum
	temp3 = AngVel * AngVel;
	rotSpeed = sqrt(temp3.sum());
	this->RotateLocal( rotSpeed * dt/mass );	
	this->ComputeInertia();
#endif

}

void VNEObject::TiltIncrementAxisX( double dVal )
{
	AngVel[0] += dVal;
	AngVel = AngVel / sqrt( AngVel[0]*AngVel[0] + AngVel[1]*AngVel[1] + AngVel[2]*AngVel[2] );
}
void VNEObject::TiltIncrementAxisY( double dVal )
{
	AngVel[1] += dVal;
	AngVel = AngVel / sqrt( AngVel[0]*AngVel[0] + AngVel[1]*AngVel[1] + AngVel[2]*AngVel[2] );
}
void VNEObject::TiltIncrementAxisZ( double dVal )
{
	AngVel[2] += dVal;
	AngVel = AngVel / sqrt( AngVel[0]*AngVel[0] + AngVel[1]*AngVel[1] + AngVel[2]*AngVel[2] );
}

void VNEObject::IncrementVelocity( const valarray<double> &plus )
{
	Velocity = Velocity + plus;
}

void VNEObject::IncrementVelocity( double dx, double dy, double dz )
{
	Velocity[0] += dx;
	Velocity[1] += dy;
	Velocity[2] += dz;

}

void VNEObject::IncrementAngVel( double dx )
{
	this->rotSpeed += dx;
	if( abs(rotSpeed) < 1e-1 )
		rotSpeed = 1e-1;
	temp3 = AngVel * AngVel;
	AngVel = rotSpeed * AngVel / sqrt(temp3.sum());
}

int VNEObject::RotateLocal( double dangle )
{
	double x,y,z;
	x = this->AngVel[0];
	y = this->AngVel[1];
	z = this->AngVel[2];

	// renormalize to account for round off / error accumulation issues
	double mag = sqrt(x*x + y*y + z*z);
	x /= mag;
	y /= mag;
	z /= mag;

	double c1 = 1 - cos( dangle );
	double dS = sin( dangle );
	double dC = cos( dangle );

	RotationMatrix[0] = x*x*c1+dC;
	RotationMatrix[1] = x*y*c1 - z*dS;
	RotationMatrix[2] = x*z*c1+y*dS;
	RotationMatrix[3] = x*y*c1+z*dS;
	RotationMatrix[4] = y*y*c1+dC;
	RotationMatrix[5] = y*z*c1-x*dS;
	RotationMatrix[6] = x*z*c1-y*dS;
	RotationMatrix[7] = y*z*c1+x*dS;
	RotationMatrix[8] = z*z*c1+dC;
	
	int i;
	double vx,vy,vz;
	for( i = 0; i < this->numVerts; i++ )
	{
		vx = CurTriVertX[i] - Centroid[0];
		vy = CurTriVertY[i] - Centroid[1];
		vz = CurTriVertZ[i] - Centroid[2];
		
		this->CurTriVertX[i] = RotationMatrix[0] * vx + RotationMatrix[1] * vy + RotationMatrix[2] * vz + Centroid[0];
		this->CurTriVertY[i] = RotationMatrix[3] * vx + RotationMatrix[4] * vy + RotationMatrix[5] * vz + Centroid[1];
		this->CurTriVertZ[i] = RotationMatrix[6] * vx + RotationMatrix[7] * vy + RotationMatrix[8] * vz + Centroid[2];
	}
	
	ComputeCentroid();

	return 0;
}

int VNEObject::TiltAxisTo( double dx, double dy, double dz )
{
	
	int iRet = 0;
	
	AngVel[0] = dx;
	AngVel[1] = dy;
	AngVel[2] = dz;

	temp3 = AngVel*AngVel;
	this->rotSpeed = sqrt(temp3.sum());

	return iRet;


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

int VNEObject::ComputeCentroid()
{
	double cx = 0.0;
	double cy = 0.0;
	double cz = 0.0;

	int i;
	for( i = 0; i < this->numVerts; i++ )
	{
		cx += CurTriVertX[i];
		cy += CurTriVertY[i];
		cz += CurTriVertZ[i];
	}
	cx /= numVerts;
	cy /= numVerts;
	cz /= numVerts;

	Centroid[0] = cx;
	Centroid[1] = cy;
	Centroid[2] = cz;

	LocTriVertX = CurTriVertX - cx;
	LocTriVertY = CurTriVertY - cy;
	LocTriVertZ = CurTriVertZ - cz;

	return 0;
}

void VNEObject::GetMinMaxVert(	double* minX, double* maxX, double* minY,
								double*  maxY, double* minZ, double* maxZ,
								int* minXi, int* maxXi, int* minYi, 
								int* maxYi, int* minZi, int* maxZi )
{


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

	return ComputeCentroid();
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

	return ComputeCentroid();
	
}

void VNEObject::ApplyInstTorque( double dx, double dy, double dz )
{
	// d/dt Iw = torque
	// w_new = w_old + time_step * torque

	

}

void VNEObject::ApplyInstTorque( const valarray<double> &Torque )
{
	// d/dt Iw = torque
	// w_new = w_old + time_step * torque

	// deltaW = I \ timestep * Torque

	temp3B = TIMESTEP * Torque;  // RHS
	temp3 = temp3B;  // initial guess

#define CHECKGS_NO
	
	for( int i = 0; i < 5; i++ )
	{ // cross fingers and hope 5 Gauss-Seidel iterations work :-)
#ifdef CHECKGS
		valarray<double> prev(3);
		prev = temp3;
#endif
		temp3[0] = ( temp3B[0] - InertiaTensor[1]*temp3[1] - InertiaTensor[2]*temp3[2] ) / InertiaTensor[0];
		temp3[1] = ( temp3B[1] - InertiaTensor[3]*temp3[0] - InertiaTensor[5]*temp3[2] ) / InertiaTensor[4];
		temp3[2] = ( temp3B[2] - InertiaTensor[6]*temp3[0] - InertiaTensor[7]*temp3[1] ) / InertiaTensor[8];

#ifdef CHECKGS
		valarray<double> acc(3);
		acc = temp3 - prev;
		acc = acc * acc;
		double diff = sqrt(acc.sum());
		prev = temp3;
#endif
	}

	this->AngVel = AngVel + temp3;
	temp3 = AngVel * AngVel;
	this->rotSpeed = sqrt(temp3.sum());

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
	this->numFaces = 0; // default before data read

	AngVel[0] = 0.0;
	AngVel[1] = 1.0;
	AngVel[2] = 0.0;

	cout<<"Reading mesh data...\n";

	numFaces = ReadMeshData( CurTriIdx, CurTriVertX, CurTriVertY, CurTriVertZ,
							 CurTriNormX, CurTriNormY, CurTriNormZ, 
							 fileNameFaces, fileNameVerts, fileNameNorms );

	numVerts = CurTriVertX.size();
	this->InertiaTensor = valarray<double>(9); // I, the 3x3 inertia tensor
	this->MassDistribution = valarray<double>(numVerts); // how much mass at each vertex is there
	this->MyTemp = valarray<double>(numVerts); // temp var for inertia computations
	this->LocTriVertX = valarray<double>(numVerts);
	this->LocTriVertY = valarray<double>(numVerts);
	this->LocTriVertZ = valarray<double>(numVerts);
	MassDistribution = mass / (numVerts); // uniform mass distribution

	ComputeCentroid();
	
	radSquared=0.0;

	int i;
	for( i = 0 ; i < numVerts; i++ )
	{
		radSquared += (CurTriVertX[i] - Centroid[0])*(CurTriVertX[i] - Centroid[0]) + (CurTriVertY[i] - Centroid[1])*(CurTriVertY[i] - Centroid[1]) + (CurTriVertZ[i] - Centroid[2])*(CurTriVertZ[i] - Centroid[2]);
	}
	radSquared /= numVerts;	
	this->bHasTexture=false;
}
void VNEObject::setTexture(string fileName)
{
	this->objTexture=new VNETexture(fileName);
	this->bHasTexture=true;
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