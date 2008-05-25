#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "VNEObject.h"
#include "CVector.h"
#include "CMatrix.h"
#include "RotationMatrix.h"
#include "IOUtils.h"
#include "MathUtils.h"


int VNEObject::DrawSelf()
{
	int coordSize = this->numFaces * 9;
	int iRet = 0;
	double* verts = new double[coordSize];
	double* norms = new double[coordSize];

	int i;
	for( i = 0; i < coordSize; i+= 3 )
	{
		verts[i]   = this->CurTriVert->GetValueAt(0,i/3);
		verts[i+1] = this->CurTriVert->GetValueAt(1,i/3);
		verts[i+2] = this->CurTriVert->GetValueAt(2,i/3);
		norms[i]   = this->CurTriNorm->GetValueAt(0,i/3);
		norms[i+1] = this->CurTriNorm->GetValueAt(1,i/3);
		norms[i+2] = this->CurTriNorm->GetValueAt(2,i/3);
	}
	glPushMatrix();
	double mx, my, mz;
	double cx, cy, cz;
	this->Centroid->GetValueAt( &cx, &cy, &cz );
	this->Moment->GetValueAt( &mx, &my, &mz );
	glTranslatef( cx, cy, cz );
	glRotatef( angularVelocity * 0.1/mass + dCurrAngle, mx, my, mz );
	dCurrAngle = angularVelocity * 0.1/mass + dCurrAngle;
	glTranslatef( -cx, -cy, -cz );
	if(this->bHasTexture)
	{	this->objTexture->bindTexture();
		for( i = 0; i < numFaces; i++ )
		{
	
			double offset = this->colorVariance*( i / double(numFaces) - 0.5 );
			glColor3f( rseed+offset, gseed+offset,bseed+offset);
			glBegin(GL_TRIANGLES);
				glNormal3d(norms[i*9+0+0], norms[i*9+0+1], norms[i*9+0+2]);
				glTexCoord2f(0.0, 0.0);
				glVertex3d(verts[i*9+0+0], verts[i*9+0+1], verts[i*9+0+2]);
				
				glNormal3d(norms[i*9+3+0], norms[i*9+3+1], norms[i*9+3+2]);
				glTexCoord2f(1.0, 1.0);
				glVertex3d(verts[i*9+3+0], verts[i*9+3+1], verts[i*9+3+2]);
				
				glNormal3d(norms[i*9+6+0], norms[i*9+6+1], norms[i*9+6+2]);
				glTexCoord2f(0.0, 1.0);
				glVertex3d(verts[i*9+6+0], verts[i*9+6+1], verts[i*9+6+2]); 
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
				glNormal3d(norms[i*9+0+0], norms[i*9+0+1], norms[i*9+0+2]);
				glVertex3d(verts[i*9+0+0], verts[i*9+0+1], verts[i*9+0+2]);
				glNormal3d(norms[i*9+3+0], norms[i*9+3+1], norms[i*9+3+2]);
				glVertex3d(verts[i*9+3+0], verts[i*9+3+1], verts[i*9+3+2]);
				glNormal3d(norms[i*9+6+0], norms[i*9+6+1], norms[i*9+6+2]);
				glVertex3d(verts[i*9+6+0], verts[i*9+6+1], verts[i*9+6+2]); 
			glEnd();
		}
	}
	glPopMatrix();
	glFinish();
	delete [] verts;
	delete [] norms;

	return iRet;
}
void VNEObject::SetSpeed( double dSpeed )
{ 
	double vx, vy, vz;
	this->Velocity->GetValueAt( &vx, &vy, &vz );
	double dnorm = sqrt( vx*vx + vy*vy + vz*vz );
	if( dSpeed > 50.0 )
	{
		dSpeed = 50.0;
		//cout<<"Max Speed Reached!\n";
	}
	else if( dSpeed < 1e-6 )
		dSpeed = 1e-6;
	this->Velocity->SetValues( vx/dnorm*dSpeed,vy/dnorm*dSpeed,vz/dnorm*dSpeed );
}

void VNEObject::PrintSelf()
{
#ifdef _DEBUG
	cout<<"Hi, I am VNEObject named: "<<this->objName<<" \n ";
#endif
}

void VNEObject::GetCentroid(double *dx, double* dy, double* dz)
{
	this->Centroid->GetValueAt(dx,dy,dz);
}

CVector* VNEObject::GetCentroid(){
	return this->Centroid;
}

void VNEObject::IncrementTime()
{
	double dt = .01;
	elapsedTime += dt;
	double vx, vy, vz;
	
	this->Velocity->GetValueAt(0, &vx);
	this->Velocity->GetValueAt(1, &vy);
	this->Velocity->GetValueAt(2, &vz);
	

	this->TranslateBy( vx*dt/mass, vy*dt/mass, vz*dt/mass );
	// CHANGE on MAY 23: use OpenGL Rotation call instead of this
	//this->RotateLocal( angularVelocity * dt/mass );	

}

void VNEObject::TiltIncrementAxisX( double dVal )
{
	double mx,my,mz;
	this->Moment->GetValueAt( &mx, &my, &mz );
	mx += dVal;
	Normalize(&mx,&my,&mz);
	this->Moment->SetValues(mx,my,mz);
}
void VNEObject::TiltIncrementAxisY( double dVal )
{
	double mx,my,mz;
	this->Moment->GetValueAt( &mx, &my, &mz );
	my += dVal;
	Normalize(&mx,&my,&mz);
	this->Moment->SetValues(mx,my,mz);
}
void VNEObject::TiltIncrementAxisZ( double dVal )
{
	double mx,my,mz;
	this->Moment->GetValueAt( &mx, &my, &mz );
	mz += dVal;
	Normalize(&mx,&my,&mz);
	this->Moment->SetValues(mx,my,mz);
}
	

int VNEObject::IncrementVelocity( double dx, double dy, double dz )
{
	double vx,vy,vz;
	this->Velocity->GetValueAt( &vx, &vy, &vz );
	vx = vx + dx;
	vy = vy + dy;
	vz = vz + dz;
	return this->Velocity->SetValues(vx,vy,vz);
}
void VNEObject::IncrementAngVel( double dx )
{
	this->angularVelocity += dx;
	if( abs(angularVelocity) < 1e-1 )
		angularVelocity = 1e-1;
}

int VNEObject::RotateLocal( double dangle )
{
	cout<<"WARNING: RotateLocal() IS ONLY FOR OCCASIONAL VERTEX LOCATION CHECKS, NOT GENERAL DRAWING!!!\n";
	// rotate by <dangle> about the vector from the centroid specified by the angular velocity
	// (rotating "about x by this, about y by that" does not make sense
	// because rotation transformations do not commute, except special cases (like 90 degrees) )

	RotationMatrix mtrx( this->Moment, dangle );
	
	// an alternate way to do this is with just a matrix multiply
	// 3x3 matrix on the left times 3 x N matrix on the right
	// it might be slightly more efficient this way... shouldn't be by much though
	// TODO: write matrix multiply function to do this (and other) operations

	// THIS NEEDS TO BE SPED UP!!!
	int i, iRet;
	double x1, y1, z1;
	double cx, cy, cz;

	this->Centroid->GetValueAt(0, &cx);
	this->Centroid->GetValueAt(1, &cy);
	this->Centroid->GetValueAt(2, &cz);
	
	CVector tempVec(3);
	CVector w_hat(3);
	double wx, wy, wz;

	for( i = 0; i < this->numFaces*3; i++ )
	{
		wx = this->CurTriVert->GetValueAt(0, i);
		wy = this->CurTriVert->GetValueAt(1, i);
		wz = this->CurTriVert->GetValueAt(2, i);
		// step1: read current vertex position into a vector
		tempVec.SetValues(wx - cx, wy - cy, wz - cz); // move origin to the centroid of object

		tempVec.MultByMatrix( &mtrx );
		tempVec.GetValueAt(&x1, &y1, &z1);

		this->CurTriVert->SetValueAt(0,i, x1 + cx);
		this->CurTriVert->SetValueAt(1,i, y1 + cy);
		this->CurTriVert->SetValueAt(2,i, z1 + cz);
		

	}
	
	iRet = ComputeCentroid();


	return iRet;

}

int VNEObject::TiltAxisBy( CVector* myVec, double dalpha )
{
int iRet = 0;

	//RotationMatrix mtrx( myVec, dalpha );
	//this->Moment->MultByMatrix( &mtrx );

	return iRet;
}

int VNEObject::TiltAxisTo( CVector* vec )
{
int iRet = 0;
	
	//iRet = this->Moment->SetValues( vec );

	return iRet;
}	

int VNEObject::TiltAxisTo( double dx, double dy, double dz )
{
	
	int iRet = 0;
	
	iRet = this->Moment->SetValues( dx,dy,dz );

	return iRet;


}

int VNEObject::ComputeCentroid()
{
	int iRet = 0;
	int i;
	double cx = 0.0;
	double cy = 0.0;
	double cz = 0.0;

	for( i = 0; i < this->numFaces*3; i++ )
	{
		cx = cx + this->CurTriVert->GetValueAt(0,i);
		cy = cy + this->CurTriVert->GetValueAt(1,i);
		cz = cz + this->CurTriVert->GetValueAt(2,i);
	}
	cx = cx / (this->numFaces * 3);
	cy = cy / (this->numFaces * 3);
	cz = cz / (this->numFaces * 3);

	
	this->Centroid->SetValueAt(0,cx);
	this->Centroid->SetValueAt(1,cy);
	this->Centroid->SetValueAt(2,cz);

	return iRet;
}

int VNEObject::TranslateTo(double dx, double dy, double dz)
{ // warning: this probably breaks now except at object initialization
	int i = 0;
	for( i = 0; i < this->numFaces*3; i++ )
	{
		this->CurTriVert->SetValueAt(0,i,dx + this->RefTriVert->GetValueAt(0,i) );
		this->CurTriVert->SetValueAt(1,i,dy + this->RefTriVert->GetValueAt(1,i) );
		this->CurTriVert->SetValueAt(2,i,dz + this->RefTriVert->GetValueAt(2,i) );
	}

	GlobalCentCoord->SetValueAt(0, dx); 
	GlobalCentCoord->SetValueAt(1, dy);
	GlobalCentCoord->SetValueAt(2, dz);

	return ComputeCentroid();
}

int VNEObject::TranslateBy(double dx, double dy, double dz)
{
	int i = 0;
	for( i = 0; i < this->numFaces*3; i++ )
	{
		this->CurTriVert->SetValueAt(0,i,dx + this->CurTriVert->GetValueAt(0,i) );
		this->CurTriVert->SetValueAt(1,i,dy + this->CurTriVert->GetValueAt(1,i) );
		this->CurTriVert->SetValueAt(2,i,dz + this->CurTriVert->GetValueAt(2,i) );
	}

	double cx, cy, cz;
	GlobalCentCoord->GetValueAt(0, &cx);
	GlobalCentCoord->GetValueAt(1, &cy);
	GlobalCentCoord->GetValueAt(2, &cz);

	GlobalCentCoord->SetValueAt(0, dx+cx); 
	GlobalCentCoord->SetValueAt(1, dy+cz);
	GlobalCentCoord->SetValueAt(2, dz+cz);

	return ComputeCentroid();
}

int VNEObject::SetVelocityProfile(double xval, double yval, double zval, int iCode )
{
	switch (iCode)
	{
	case 0: // linear velocity
		this->Velocity->SetValueAt(0,xval);
		this->Velocity->SetValueAt(1,yval);
		this->Velocity->SetValueAt(2,zval);
		break;
	case 1: // orthogonal sinusoidal velocities
		this->Velocity->SetValueAt(0,3*cos(5*xval ) );
		this->Velocity->SetValueAt(1,3*sin(5*yval ) );
		this->Velocity->SetValueAt(2,zval);
		break;
	case 2: // a funky velocity
		this->Velocity->SetValueAt(0,6*cos(5*xval )*sin(5*xval ) );
		this->Velocity->SetValueAt(1,6*sin(5*yval )*cos(5*xval) );
		this->Velocity->SetValueAt(2,2*cos(2*zval));
		break;
	default:
		return -1;
		break;
	}
	return 0;
}

double VNEObject::GetSpeed( )
{
	double vx,vy,vz;
	this->Velocity->GetValueAt( &vx, &vy, &vz );
	return sqrt(vx*vx + vy*vy + vz*vz );
}

VNEObject::VNEObject( string objName, string fileNameFaces, string fileNameVerts, string fileNameNorms )
{
	this->objName = objName;

	elapsedTime = 0.0;
	mass = 1.0;
	dCurrAngle = 0.0;

	GlobalCentCoord = new CVector(3);
	GlobalCentCoord->SetValueAt(0,0.0);
	GlobalCentCoord->SetValueAt(1,0.0);
	GlobalCentCoord->SetValueAt(2,0.0);
	
	Velocity = new CVector(3);
	Velocity->SetValueAt(0,0.0);
	Velocity->SetValueAt(1,0.0);
	Velocity->SetValueAt(2,0.0);

	Centroid = new CVector(3);
	Centroid->SetValueAt(0,0.0);
	Centroid->SetValueAt(1,0.0);
	Centroid->SetValueAt(2,0.0);

	Moment = new CVector(3);
	Moment->SetValueAt(0,0.0);
	Moment->SetValueAt(1,1.0);
	Moment->SetValueAt(2,0.0);

	colorVariance = 0.3; // how much will colors vary from the seed
	rseed = 1.0;
	gseed = 1.0;
	bseed = 1.0;

	this->angularVelocity = 0.001;
	
	numFaces = ReadMeshData( &CurTriVert, &CurTriNorm, fileNameFaces, fileNameVerts, fileNameNorms );
	
	//that's rediculous, make a COPY CONSTRUCTOR
	ReadMeshData( &RefTriVert, &CurTriNorm, fileNameFaces, fileNameVerts, fileNameNorms );

	ComputeCentroid();
	double dVal, cVal;
	
	radSquared=0.0;
	for( int i = 0; i < this->numFaces * 3; i++ )
	{
		dVal = CurTriVert->GetValueAt(0,i);
		this->Centroid->GetValueAt(0, &cVal);
		CurTriVert->SetValueAt(0,i, dVal - cVal);
		RefTriVert->SetValueAt(0,i, dVal - cVal);
		radSquared += (dVal - cVal)*(dVal - cVal); // add this vertex's x^2
		dVal = CurTriVert->GetValueAt(1,i);
		this->Centroid->GetValueAt(1, &cVal);
		CurTriVert->SetValueAt(1,i, dVal - cVal);
		RefTriVert->SetValueAt(1,i, dVal - cVal);
		radSquared += (dVal - cVal)*(dVal - cVal); // add this vertex's y^2
		dVal = CurTriVert->GetValueAt(2,i);
		this->Centroid->GetValueAt(2, &cVal);
		CurTriVert->SetValueAt(2,i, dVal - cVal);
		RefTriVert->SetValueAt(2,i, dVal - cVal);
		radSquared += (dVal - cVal)*(dVal - cVal); // add this vertex's z^2

	}
	radSquared /= (this->numFaces * 3); // divide by number of vertices ( three times the number of faces, if it is all triangle faces )
	ComputeCentroid();
	
	
	
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
{
// default: make a regular tetrahedron centered at global origin

	elapsedTime = 0.0;

	GlobalCentCoord = new CVector(3);
	GlobalCentCoord->SetValueAt(0,0.0);
	GlobalCentCoord->SetValueAt(1,0.0);
	GlobalCentCoord->SetValueAt(2,0.0);
	
	Velocity = new CVector(3);
	Velocity->SetValueAt(0,0.0);
	Velocity->SetValueAt(1,0.0);
	Velocity->SetValueAt(2,0.0);

	Centroid = new CVector(3);
	Centroid->SetValueAt(0,0.0);
	Centroid->SetValueAt(1,0.0);
	Centroid->SetValueAt(2,0.0);

	Moment = new CVector(3);
	Moment->SetValueAt(0,0.0);
	Moment->SetValueAt(1,1.0);
	Moment->SetValueAt(2,0.0);

	this->angularVelocity = 10.0;


	// 0  1 0 | 0 1 0 | 0 0 0 | 1 0 0
 	// 0  0 1 | 0 0 0 | 0 1 0 | 0 1 0
	// 0  0 0 | 0 0 1 | 0 0 1 | 0 0 1
	//string faces = "faces1.dat";
	//string verts = "verts1.dat";
	string faces = "..\\vne_data\\faces1.dat";
	string verts = "..\\vne_data\\verts1.dat";
	//cout<<"Default Object Constructor is Deprecated!\n";
	exit(1);
	//numFaces = ReadMeshData( &CurTriVert, faces, verts );
	//ReadMeshData( &RefTriVert, faces, verts );

	//this->TranslateTo( 0.0, 0.0, 0.0);

	ComputeCentroid();
	
	double dVal, cVal;
	for( int i = 0; i < this->numFaces * 3; i++ )
	{
		dVal = CurTriVert->GetValueAt(0,i);
		this->Centroid->GetValueAt(0, &cVal);
		CurTriVert->SetValueAt(0,i, dVal - cVal);
		RefTriVert->SetValueAt(0,i, dVal - cVal);
		dVal = CurTriVert->GetValueAt(1,i);
		this->Centroid->GetValueAt(1, &cVal);
		CurTriVert->SetValueAt(1,i, dVal - cVal);
		RefTriVert->SetValueAt(1,i, dVal - cVal);
		dVal = CurTriVert->GetValueAt(2,i);
		this->Centroid->GetValueAt(2, &cVal);
		CurTriVert->SetValueAt(2,i, dVal - cVal);
		RefTriVert->SetValueAt(2,i, dVal - cVal);
	}
	ComputeCentroid();
	

}