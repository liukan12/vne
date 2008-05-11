#include "VNEObject.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

VNEObject::VNEObject()
{
// default: make a regular tetrahedron centered at global origin
	CVector GlobalCentCoord(3);
	GlobalCentCoord.SetValueAt(0,0.0);
	GlobalCentCoord.SetValueAt(1,0.0);
	GlobalCentCoord.SetValueAt(2,0.0);

	CVector Velocity(3);
	Velocity.SetValueAt(0,0.0);
	Velocity.SetValueAt(0,0.0);
	Velocity.SetValueAt(0,0.0);

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

}


int VNEObject::GetCurTriVerts(double *pdVerts)
{
// return the vertices in OpenGL vertex array format
// [x0 y0 z0 x1 y1 z1 ... ]

	return 0;
}






//#ifndef VNEOBJECT
//#define VNEOBJECT
//
//class VNEObject
//{
//private:
//	CMatrix CurTriVert; // current vertices of triangle faces w.r.t. local coordinate system
//	CMatrix RefTriVert; // original / refernce vertex coordinates
//	CVector Velocity;   // x,y,z velocity
//	
//	double md_GlobalCentCoord; // where is my local "origin" located in the global coordinate system?
//	
//
//
//public:
//	VNEObject( char* dataFileName ); // construct from a file with vertex coords
//	VNEObject( ); // default constructor (equi-sided tetrahedron ? )
//	~VNEObject( ); // destructor 
//	int GetCurTriVerts( double* pdVerts );
//	int GetRefTriVerts( double* pdVerts );
//	int Translate( double dx, double dy, double dz );
//	int RotateLocal( double anglex, double angley, double anglez );
//	// rotate locally (in-place about object centroid)
//	int RotateAbout( double xt, double xc, double yt, double yc,
//					 double zt, double zc, double angle ); 
//	// rotate about a line parametrized as z = zt * t + zc, y = yt * t + yc, x = xt * t + xc
//	// where zt, yt, xt are constants; right-handed coordinate system assumed
//	// using direction of increasing t parameter
//
//}
//
//
//
//
//
//
//
//#endif
