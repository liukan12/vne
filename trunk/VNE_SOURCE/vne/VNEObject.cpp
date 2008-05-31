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
#include "MathUtils.h"
#include <valarray>
#include <numeric>

#define MANUAL_ROTATION // do I use rotation matrix code and keep track of precise vertex locations?

using namespace std;

static valarray<double> RotationMatrix(9);
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
	CurTriVert = obj.CurTriVert;
	CurTriNorm = obj.CurTriNorm;
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
				glNormal3d(CurTriNorm[i*9+0+0], CurTriNorm[i*9+0+1], CurTriNorm[i*9+0+2]);
				glTexCoord2f(0.0, 0.0);
				glVertex3d(CurTriVert[i*9+0+0], CurTriVert[i*9+0+1], CurTriVert[i*9+0+2]);
				
				glNormal3d(CurTriNorm[i*9+3+0], CurTriNorm[i*9+3+1], CurTriNorm[i*9+3+2]);
				glTexCoord2f(1.0, 1.0);
				glVertex3d(CurTriVert[i*9+3+0], CurTriVert[i*9+3+1], CurTriVert[i*9+3+2]);
				
				glNormal3d(CurTriNorm[i*9+6+0], CurTriNorm[i*9+6+1], CurTriNorm[i*9+6+2]);
				glTexCoord2f(0.0, 1.0);
				glVertex3d(CurTriVert[i*9+6+0], CurTriVert[i*9+6+1], CurTriVert[i*9+6+2]); 
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
				glNormal3d(CurTriNorm[i*9+0+0], CurTriNorm[i*9+0+1], CurTriNorm[i*9+0+2]);
				glVertex3d(CurTriVert[i*9+0+0], CurTriVert[i*9+0+1], CurTriVert[i*9+0+2]);
				glNormal3d(CurTriNorm[i*9+3+0], CurTriNorm[i*9+3+1], CurTriNorm[i*9+3+2]);
				glVertex3d(CurTriVert[i*9+3+0], CurTriVert[i*9+3+1], CurTriVert[i*9+3+2]);
				glNormal3d(CurTriNorm[i*9+6+0], CurTriNorm[i*9+6+1], CurTriNorm[i*9+6+2]);
				glVertex3d(CurTriVert[i*9+6+0], CurTriVert[i*9+6+1], CurTriVert[i*9+6+2]); 
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

valarray<double> VNEObject::GetCentroid(){
	return this->Centroid;
}

void VNEObject::IncrementTime()
{
	double dt = .01;
	elapsedTime += dt;
		
	this->TranslateBy( Velocity[0]*dt/mass, Velocity[1]*dt/mass, Velocity[2]*dt/mass );
	// CHANGE on MAY 23: use OpenGL Rotation call instead of this
#ifdef MANUAL_ROTATION
	// this still needs to happen potentially if we want highly accurate collision handling
	// with angular momentum
	this->RotateLocal( rotSpeed * dt/mass );	
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
	

int VNEObject::IncrementVelocity( double dx, double dy, double dz )
{
	valarray<double> plus(3);
	plus[0] = dx;
	plus[1] = dy;
	plus[2] = dz;

	Velocity = Velocity + plus;
	
	return 0;
}
void VNEObject::IncrementAngVel( double dx )
{
	this->rotSpeed += dx;
	if( abs(rotSpeed) < 1e-1 )
		rotSpeed = 1e-1;
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
	for( i = 0; i < this->numFaces*3; i++ )
	{
		vx = CurTriVert[i*3 + 0] - Centroid[0];
		vy = CurTriVert[i*3 + 1] - Centroid[1];
		vz = CurTriVert[i*3 + 2] - Centroid[2];
		
		this->CurTriVert[i*3 + 0] = RotationMatrix[0] * vx + RotationMatrix[1] * vy + RotationMatrix[2] * vz + Centroid[0];
		this->CurTriVert[i*3 + 1] = RotationMatrix[3] * vx + RotationMatrix[4] * vy + RotationMatrix[5] * vz + Centroid[1];
		this->CurTriVert[i*3 + 2] = RotationMatrix[6] * vx + RotationMatrix[7] * vy + RotationMatrix[8] * vz + Centroid[2];
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

	return iRet;


}

int VNEObject::ComputeCentroid()
{
	double cx = 0.0;
	double cy = 0.0;
	double cz = 0.0;

	int i;
	for( i = 0; i < this->numFaces*3; i++ )
	{
		cx += CurTriVert[i*3+0];
		cy += CurTriVert[i*3+1];
		cz += CurTriVert[i*3+2];
	}
	cx /= (numFaces*3);
	cy /= (numFaces*3);
	cz /= (numFaces*3);

	Centroid[0] = cx;
	Centroid[1] = cy;
	Centroid[2] = cz;

	return 0;
}

int VNEObject::TranslateTo(double dx, double dy, double dz)
{ // warning: this probably breaks now except at object initialization
	int i;
	for( i = 0; i < this->numFaces*3; i++ )
	{
		this->CurTriVert[i*3 + 0] = dx - Centroid[0] + CurTriVert[i*3 + 0];
		this->CurTriVert[i*3 + 1] = dx - Centroid[1] + CurTriVert[i*3 + 1];
		this->CurTriVert[i*3 + 2] = dx - Centroid[2] + CurTriVert[i*3 + 2];
	}

	return ComputeCentroid();
}

int VNEObject::TranslateBy(double dx, double dy, double dz)
{
	int i;
	for( i = 0 ; i < this->numFaces*3 ; i++ )
	{
		CurTriVert[3*i] += dx;
		CurTriVert[3*i+1] += dy;
		CurTriVert[3*i+2] += dz;
	}

	return ComputeCentroid();
	
}

int VNEObject::SetVelocityProfile(double xval, double yval, double zval, int iCode )
{
	
		this->Velocity[0] = xval;
		this->Velocity[1] = yval;
		this->Velocity[2] = zval;
	
	return 0;
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

	// TODO: USE THE FACES DATA FILE IN OPENGL CALLS! Eliminate redundant vertices !!!!!
	numFaces = ReadMeshData( &CurTriVert, &CurTriNorm, fileNameFaces, fileNameVerts, fileNameNorms );
	
	this->InertiaTensor = valarray<double>(9); // I, the 3x3 inertia tensor
	this->MassDistribution = valarray<double>(numFaces*3); // how much mass at each vertex is there
	MassDistribution = mass / (numFaces * 3); // "uniform" distribution

	ComputeCentroid();
	
	radSquared=0.0;

	int i;
	for( i = 0 ; i < numFaces*3; i++ )
	{
		radSquared += (CurTriVert[i*3+0] - Centroid[0])*(CurTriVert[i*3+0] - Centroid[0]) + (CurTriVert[i*3+1] - Centroid[1])*(CurTriVert[i*3+1] - Centroid[1]) + (CurTriVert[i*3+2] - Centroid[2])*(CurTriVert[i*3+2] - Centroid[2]);
	}
	radSquared /= numFaces * 3;	
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