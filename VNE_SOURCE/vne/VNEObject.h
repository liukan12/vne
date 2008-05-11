#ifndef VNEOBJECT
#define VNEOBJECT

#include "CMatrix.h"
#include "CVector.h"

class VNEObject
{
private:
	CMatrix* CurTriVert; // current vertices of triangle faces w.r.t. local coordinate system
	CMatrix* RefTriVert; // original / refernce vertex coordinates
	CVector Velocity;   // x,y,z velocity
	CVector GlobalCentCoord; // where is my local "origin" located in the global coordinate system?
	int numFaces;

public:
	VNEObject( char* dataFileName ); // construct from a file with vertex coords
	VNEObject( ); // default constructor (equi-sided tetrahedron ? )
	~VNEObject( ); // destructor 
	int GetNumFaces(){ return numFaces; }
	int GetCurTriVerts( double* pdVerts );
	int GetRefTriVerts( double* pdVerts );
	int Translate( double dx, double dy, double dz );
	int RotateLocal( double anglex, double angley, double anglez );
	// rotate locally (in-place about object centroid)
	int RotateAbout( double xt, double xc, double yt, double yc,
					 double zt, double zc, double angle ); 
	// rotate about a line parametrized as z = zt * t + zc, y = yt * t + yc, x = xt * t + xc
	// where zt, yt, xt are constants; right-handed coordinate system assumed
	// using direction of increasing t parameter

};







#endif
