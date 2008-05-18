#ifndef VNEOBJECT
#define VNEOBJECT

#include "CMatrix.h"
#include "CVector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

class VNEObject
{
private:
	CMatrix* CurTriVert; // current vertices of triangle faces w.r.t. local coordinate system
	CMatrix* RefTriVert; // original / refernce vertex coordinates
	CVector* Velocity;   // x,y,z velocity
	CVector* Moment;   // x,y,z angular velocity
	CVector* GlobalCentCoord; // where is my local "origin" located in the global coordinate system?
	CVector* Centroid; // the euclidean center of the object;
	int numFaces;
	string objName;
	double speedFactor;
	double unitDrift1; // parameter for applying "brownian motion"
	double unitDrift2;
	double elapsedTime;
	double angularVelocity;

	int ComputeCentroid();

public:
	VNEObject( string objName, string fileNameFaces, string fileNameVerts ); // construct from a file with vertex coords
	VNEObject( string objName );
	VNEObject( ); // default constructor (equi-sided tetrahedron ? )
	~VNEObject( ); // destructor 
	int DrawSelf();
	void PrintSelf();
	void IncrementTime( );
	double GetSpeed( ) { return speedFactor; }
	double GetAngVel( ) { return angularVelocity; }
	double GetElapsedTime() { return elapsedTime; };
	int GetNumFaces(){ return numFaces; }
	CVector GetCentroid();
	int SpinAboutCentroid();
	void SetSpeed( double dSpeed ) { speedFactor = dSpeed; }
	void SetAngularVelocity( double dAngVel ) { angularVelocity = dAngVel; }
	int TiltAxisBy( CVector* vec, double dalpha );
	int TiltAxisTo( CVector* vec );
	int TiltAxisTo( double dx, double dy, double dz );
	int TranslateTo( double dx, double dy, double dz );
	int TranslateBy( double dx, double dy, double dz );
	int RotateLocal( double dangle  );
	// rotate locally (in-place about object centroid)
	int RotateAbout( double xt, double xc, double yt, double yc,
					 double zt, double zc, double angle );
	int SetVelocityProfile(double xval, double yval, double zval, int iCode );
	// rotate about a line parametrized as z = zt * t + zc, y = yt * t + yc, x = xt * t + xc
	// where zt, yt, xt are constants; right-handed coordinate system assumed
	// using direction of increasing t parameter

};







#endif
