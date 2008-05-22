#ifndef VNEOBJECT
#define VNEOBJECT

#include "CMatrix.h"
#include "CVector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>

#define COLORSEED 0 // given RGB seed values, randomly perturb around them
#define COLORREAD 1 // per-vertex designated coloring, read from file or otherwise input

using namespace std;

class VNEObject
{
private:
	CMatrix* CurTriVert; // current vertices of triangle faces w.r.t. local coordinate system
	CMatrix* RefTriVert; // original / refernce vertex coordinates
	CMatrix* CurTriNorm; // normals at vertices
	CVector* Velocity;   // x,y,z velocity
	CVector* Moment;   // x,y,z angular velocity
	CVector* GlobalCentCoord; // where is my local "origin" located in the global coordinate system?
	CVector* Centroid; // the euclidean center of the object;
	double rseed, gseed, bseed; // RGB seeds for coloring the vertices
	double colorVariance;
	double mass;
	int numFaces;
	string objName;
	double speedFactor;
	double unitDrift1; // parameter for applying "brownian motion"
	double unitDrift2;
	double elapsedTime;
	double angularVelocity;

	int ComputeCentroid();

public:
	VNEObject( string objName, string fileNameFaces, string fileNameVerts, string fileNameNorms ); // construct from a file with vertex coords
	VNEObject( string objName );
	VNEObject( ); // default constructor (equi-sided tetrahedron ? )
	~VNEObject( ); // destructor 
	int DrawSelf();
	void PrintSelf();
	void IncrementTime( );
	double GetMass( ) { return mass; }
	double GetSpeed( );
	double GetAngVel( ) { return angularVelocity; }
	double GetElapsedTime() { return elapsedTime; };
	int GetNumFaces(){ return numFaces; }
	void GetCentroid(double *dx, double* dy, double* dz);
	int SpinAboutCentroid();
	void SetColorSeed( double r, double g, double b );
	void GetColorSeed( double* r, double* g, double* b);
	void SetSpeed( double dSpeed );
	void SetAngularVelocity( double dAngVel ) { angularVelocity = dAngVel; }
	int TiltAxisBy( CVector* vec, double dalpha );
	int TiltAxisTo( CVector* vec );
	int TiltAxisTo( double dx, double dy, double dz );
	int TranslateTo( double dx, double dy, double dz );
	int TranslateBy( double dx, double dy, double dz );
	int RotateLocal( double dangle  );
	CVector* GetVelocity() { return Velocity; }
	int IncrementVelocity( double dx, double dy, double dz );
	// rotate locally (in-place about object centroid)
	int RotateAbout( double xt, double xc, double yt, double yc,
					 double zt, double zc, double angle );
	int SetVelocityProfile(double xval, double yval, double zval, int iCode );
	// rotate about a line parametrized as z = zt * t + zc, y = yt * t + yc, x = xt * t + xc
	// where zt, yt, xt are constants; right-handed coordinate system assumed
	// using direction of increasing t parameter

};







#endif
