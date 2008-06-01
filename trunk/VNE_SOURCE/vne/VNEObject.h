#ifndef VNEOBJECT
#define VNEOBJECT

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <valarray>
#include <numeric>
#include "VNETexture.h"

#define COLORSEED 0 // given RGB seed values, randomly perturb around them
#define COLORREAD 1 // per-vertex designated coloring, read from file or otherwise input
#define TIMESTEP 0.01

using namespace std;

class VNEObject
{
public: // maybe dangerous, but can stop passing around things with function calls and access memory directly -> FASTER!!!
	// mesh properties
	valarray<double> CurTriVertX; // current vertices of triangle faces w.r.t. local coordinate system
	valarray<double> CurTriVertY; // current vertices of triangle faces w.r.t. local coordinate system
	valarray<double> CurTriVertZ; // current vertices of triangle faces w.r.t. local coordinate system

	valarray<double> CurTriNormX; // normals at vertices
	valarray<double> CurTriNormY; // normals at vertices
	valarray<double> CurTriNormZ; // normals at vertices
	
	valarray<int>	 CurTriIdx; // indices into CurTriVert and CurTriNorm

	// coordinates referenced to centroid
	valarray<double> LocTriVertX; // 
	valarray<double> LocTriVertY; // 
	valarray<double> LocTriVertZ; // 
	
	// physics properties
	valarray<double> Velocity;   // x,y,z instantaneous velocity
	valarray<double> AngVel;   // x,y,z instantaneous angular velocity
	valarray<double> Centroid; // the euclidean center of the object;
	valarray<double> InertiaTensor; // I, the 3x3 inertia tensor
	valarray<double> MassDistribution; // how much mass at each vertex is there
	valarray<double> MyTemp; // allocate one temporary array for inertia computations

	VNETexture* objTexture;
	
	double rseed, gseed, bseed; // RGB seeds for coloring the vertices
	double colorVariance;
	double mass;
	int numFaces;
	int numVerts;
	string objName;
	
	double radSquared;
	double elapsedTime;
	double rotSpeed;

	int ComputeCentroid();
	void ComputeInertia();
	bool bHasTexture;
	bool bIsStatic;//is object affected by global forces

public:
	VNEObject(const VNEObject& obj);
	VNEObject& operator=(const VNEObject& rhs);
	void CopyObj(const VNEObject& obj);

	VNEObject( string objName, string fileNameFaces, string fileNameVerts, string fileNameNorms ); // construct from a file with vertex coords
	VNEObject( string objName );
	VNEObject( ); // default constructor (equi-sided tetrahedron ? )
	~VNEObject( ); // destructor
	
	void setTexture(string fileName);
	void setTexture(VNETexture* newTex);
	int DrawSelf();
	void PrintSelf();
	void IncrementTime( );
	//int SelectQuery(int iListIdx);
	
	void GetMinMaxVert(	double* minX, double* maxX, double* minY,
								double*  maxY, double* minZ, double* maxZ,
								int* minXi, int* maxXi, int* minYi, 
								int* maxYi, int* minZi, int* maxZi );
	
	int GetNumVerts( ) { return numVerts; }
	double GetMass( ) { return mass; }
	double GetSpeed( );
	double GetAngVelMag( ) { return rotSpeed; }
	double GetElapsedTime() { return elapsedTime; };
	double GetRadSquared(){return radSquared;}
	int GetNumFaces(){ return numFaces; }
	void GetCentroid(double *dx, double* dy, double* dz);
	valarray<double>* GetCentroid();
	int SpinAboutCentroid();
	void SetColorSeed( double r, double g, double b );
	void GetColorSeed( double* r, double* g, double* b);
	void SetSpeed( double dSpeed );
	void SetRotSpeed( double dAngVel );
	void ApplyInstTorque( const valarray<double> &torque );
	void ApplyInstTorque( double dx, double dy, double dz );
	int TiltAxisBy( valarray<double> &vec, double dalpha );
	int TiltAxisBy( double dx, double dy, double dz );
	int TiltAxisTo( valarray<double> &vec );
	int TiltAxisTo( double dx, double dy, double dz );
	void TiltIncrementAxisX( double dVal );
	void TiltIncrementAxisY( double dVal );
	void TiltIncrementAxisZ( double dVal );
	int TranslateTo( double dx, double dy, double dz );
	int TranslateBy( double dx, double dy, double dz );
	void IncrementAngVel( double dx );
	int RotateLocal( double dangle  );
	valarray<double>* GetVelocity() { return  &Velocity; }
	void IncrementVelocity( const valarray<double> &plus );
	void IncrementVelocity( double dx, double dy, double dz );
	// rotate locally (in-place about object centroid)
	int RotateAbout( double xt, double xc, double yt, double yc,
					 double zt, double zc, double angle );
	void SetVelocityProfile(double xval, double yval, double zval, int iCode );
	// rotate about a line parametrized as z = zt * t + zc, y = yt * t + yc, x = xt * t + xc
	// where zt, yt, xt are constants; right-handed coordinate system assumed
	// using direction of increasing t parameter

};







#endif

