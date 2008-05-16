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


int VNEObject::DrawSelf()
{
	int coordSize = this->numFaces * 9;
	int iRet = 0;
	double* verts = new double[coordSize];

	int i;
	for( i = 0; i < coordSize; i+= 3 )
	{
		verts[i] = this->CurTriVert->GetValueAt(0,i/3);
		verts[i+1] = this->CurTriVert->GetValueAt(1,i/3);
		verts[i+2] = this->CurTriVert->GetValueAt(2,i/3);
	}

	glPushMatrix();

	for( i = 0; i < numFaces; i++ )
	{
		glColor3f( ((float)i)/numFaces, 1.0 - ((float)i)/numFaces,((float)i)/numFaces);
		glBegin(GL_TRIANGLES);
			glVertex3d(verts[i*9+0+0], verts[i*9+0+1], verts[i*9+0+2]);
			glVertex3d(verts[i*9+3+0], verts[i*9+3+1], verts[i*9+3+2]);
			glVertex3d(verts[i*9+6+0], verts[i*9+6+1], verts[i*9+6+2]); 
		glEnd();
	}
	glPopMatrix();
	delete [] verts;

	return iRet;
}

void VNEObject::PrintSelf()
{
	cout<<"Hi, I am VNEObject named: "<<this->objName<<" \n ";
}

void VNEObject::IncrementTime()
{
	double dt = .01;
	elapsedTime += dt;
	
	int iVcode = 1; // velocity code; later this needs to be input from a script somewhere
	this->SetVelocityProfile( elapsedTime, elapsedTime, 0.0, 1 );

	double vx, vy, vz;
	this->Velocity->GetValueAt(0, &vx);
	this->Velocity->GetValueAt(1, &vy);
	this->Velocity->GetValueAt(2, &vz);
	
	this->TranslateBy( vx*dt, vy*dt, vz*dt );
	this->RotateLocal( angularVelocity * dt );

	// not sure if this is right, it is pretty hard to verify (complex motion)
	//CVector spin( sin(elapsedTime), cos(elapsedTime), 0.0 );
	//this->TiltAxisBy( &spin, dt );
	// now perform a spin according to current angular velocity

	

}

int VNEObject::RotateLocal( double dangle )
{
	// rotate by <dangle> about the vector from the centroid specified by the angular velocity
	// (rotating "about x by this, about y by that" does not make sense
	// because rotation transformations do not commute, except special cases (like 90 degrees) )

	RotationMatrix mtrx( this->Moment, dangle );
	
	// an alternate way to do this is with just a matrix multiply
	// 3x3 matrix on the left times 3 x N matrix on the right
	// it might be slightly more efficient this way... shouldn't be by much though
	// TODO: write matrix multiply function to do this (and other) operations

	CVector tempVec(3);
	int i, iRet;
	double x1, y1, z1;
	double cx, cy, cz;

	this->Centroid->GetValueAt(0, &cx);
	this->Centroid->GetValueAt(1, &cy);
	this->Centroid->GetValueAt(2, &cz);

	for( i = 0; i < this->numFaces*3; i++ )
	{
		// step1: read current vertex position into a vector
		tempVec.SetValueAt(0, this->CurTriVert->GetValueAt(0, i) - cx );
		tempVec.SetValueAt(1, this->CurTriVert->GetValueAt(1, i) - cy );
		tempVec.SetValueAt(2, this->CurTriVert->GetValueAt(2, i) - cz );

		tempVec.MultByMatrix( &mtrx );
		tempVec.GetValueAt(0, &x1);
		tempVec.GetValueAt(1, &y1);
		tempVec.GetValueAt(2, &z1);

		this->CurTriVert->SetValueAt(0,i, x1 + cx);
		this->CurTriVert->SetValueAt(1,i, y1 + cy);
		this->CurTriVert->SetValueAt(2,i, z1 + cz);
	}
	
	iRet = ComputeCentroid();

	// debug check: rotation should NOT affect the centroid (seems like it is OK...)
	//double cx2, cy2, cz2;
	//this->Centroid->GetValueAt(0, &cx2);
	//this->Centroid->GetValueAt(1, &cy2);
	//this->Centroid->GetValueAt(2, &cz2);


	return iRet;

}

int VNEObject::TiltAxisBy( CVector* myVec, double dalpha )
{
int iRet = 0;

	RotationMatrix mtrx( myVec, dalpha );
	this->Moment->MultByMatrix( &mtrx );

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
{
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

int VNEObject::SpinAboutCentroid()
{
	double dx, dy, dz;
	double cx, cy, cz;
	GlobalCentCoord->GetValueAt(0, &dx);
	GlobalCentCoord->GetValueAt(1, &dy);
	GlobalCentCoord->GetValueAt(2, &dz);
	Centroid->GetValueAt(0, &cx);
	Centroid->GetValueAt(1, &cy);
	Centroid->GetValueAt(2, &cz);

	dx = dx - cx;
	dy = dy - cy;
	dz = dz - cz;
	
	
	unitDrift1 += (rand()%7)/6.0 - 3.0;

	return 0;

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
		this->Velocity->SetValueAt(0,6*cos(5*xval) );
		this->Velocity->SetValueAt(1,6*sin(5*yval) );
		this->Velocity->SetValueAt(2,zval);
		break;
	default:
		return -1;
		break;
	}
	return 0;
}

VNEObject::VNEObject()
{
// default: make a regular tetrahedron centered at global origin
	unitDrift1 = 45.0;
	unitDrift2 = 0.0;
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

	this->numFaces = 4;

	// 0  1 0 | 0 1 0 | 0 0 0 | 1 0 0
 	// 0  0 1 | 0 0 0 | 0 1 0 | 0 1 0
	// 0  0 0 | 0 0 1 | 0 0 1 | 0 0 1

	RefTriVert = new CMatrix(3, 12); // each 3x3 block will define one triangle face
	RefTriVert->SetValueAt(0,0,0.0); RefTriVert->SetValueAt(1,0,0.0); RefTriVert->SetValueAt(2,0,0.0);
	RefTriVert->SetValueAt(0,1,1.0); RefTriVert->SetValueAt(1,1,0.0); RefTriVert->SetValueAt(2,1,0.0);
	RefTriVert->SetValueAt(0,2,0.0); RefTriVert->SetValueAt(1,2,1.0); RefTriVert->SetValueAt(2,2,0.0);

	RefTriVert->SetValueAt(0,3,0.0); RefTriVert->SetValueAt(1,3,0.0); RefTriVert->SetValueAt(2,3,0.0);
	RefTriVert->SetValueAt(0,4,1.0); RefTriVert->SetValueAt(1,4,0.0); RefTriVert->SetValueAt(2,4,0.0);
	RefTriVert->SetValueAt(0,5,0.0); RefTriVert->SetValueAt(1,5,0.0); RefTriVert->SetValueAt(2,5,1.0);

	RefTriVert->SetValueAt(0,6,0.0); RefTriVert->SetValueAt(1,6,0.0); RefTriVert->SetValueAt(2,6,0.0);
	RefTriVert->SetValueAt(0,7,0.0); RefTriVert->SetValueAt(1,7,1.0); RefTriVert->SetValueAt(2,7,0.0);
	RefTriVert->SetValueAt(0,8,0.0); RefTriVert->SetValueAt(1,8,0.0); RefTriVert->SetValueAt(2,8,1.0);

	RefTriVert->SetValueAt(0,9 ,1.0); RefTriVert->SetValueAt(1,9 ,0.0); RefTriVert->SetValueAt(2,9 ,0.0);
	RefTriVert->SetValueAt(0,10,0.0); RefTriVert->SetValueAt(1,10,1.0); RefTriVert->SetValueAt(2,10,0.0);
	RefTriVert->SetValueAt(0,11,0.0); RefTriVert->SetValueAt(1,11,0.0); RefTriVert->SetValueAt(2,11,1.0);

	CurTriVert = new CMatrix( 3, 12 );
	CurTriVert->SetValueAt(0,0,0.0); CurTriVert->SetValueAt(1,0,0.0); CurTriVert->SetValueAt(2,0,0.0);
	CurTriVert->SetValueAt(0,1,1.0); CurTriVert->SetValueAt(1,1,0.0); CurTriVert->SetValueAt(2,1,0.0);
	CurTriVert->SetValueAt(0,2,0.0); CurTriVert->SetValueAt(1,2,1.0); CurTriVert->SetValueAt(2,2,0.0);

	CurTriVert->SetValueAt(0,3,0.0); CurTriVert->SetValueAt(1,3,0.0); CurTriVert->SetValueAt(2,3,0.0);
	CurTriVert->SetValueAt(0,4,1.0); CurTriVert->SetValueAt(1,4,0.0); CurTriVert->SetValueAt(2,4,0.0);
	CurTriVert->SetValueAt(0,5,0.0); CurTriVert->SetValueAt(1,5,0.0); CurTriVert->SetValueAt(2,5,1.0);

	CurTriVert->SetValueAt(0,6,0.0); CurTriVert->SetValueAt(1,6,0.0); CurTriVert->SetValueAt(2,6,0.0);
	CurTriVert->SetValueAt(0,7,0.0); CurTriVert->SetValueAt(1,7,1.0); CurTriVert->SetValueAt(2,7,0.0);
	CurTriVert->SetValueAt(0,8,0.0); CurTriVert->SetValueAt(1,8,0.0); CurTriVert->SetValueAt(2,8,1.0);

	CurTriVert->SetValueAt(0,9 ,1.0); CurTriVert->SetValueAt(1,9 ,0.0); CurTriVert->SetValueAt(2,9 ,0.0);
	CurTriVert->SetValueAt(0,10,0.0); CurTriVert->SetValueAt(1,10,1.0); CurTriVert->SetValueAt(2,10,0.0);
	CurTriVert->SetValueAt(0,11,0.0); CurTriVert->SetValueAt(1,11,0.0); CurTriVert->SetValueAt(2,11,1.0);

	this->TranslateTo( 0.0, 0.0, 0.0);

	ComputeCentroid();
	this->Centroid->PrintSelf();
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
	this->Centroid->PrintSelf();

}