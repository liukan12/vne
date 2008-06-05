#ifndef VNEOBJECT
#define VNEOBJECT

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <valarray>
#include <numeric>
#include "VNETexture.h"
#include "WorldForce.h"

#define COLORSEED 0 // given RGB seed values, randomly perturb around them
#define COLORREAD 1 // per-vertex designated coloring, read from file or otherwise input

using namespace std;

class VNEObject
{
public: // maybe dangerous, but can stop passing around things with function calls and access memory directly -> FASTER!!!
	// mesh properties
	valarray<double> CurTriVertX; // current vertices of triangle faces w.r.t. local coordinate system
	valarray<double> CurTriVertY; // current vertices of triangle faces w.r.t. local coordinate system
	valarray<double> CurTriVertZ; // current vertices of triangle faces w.r.t. local coordinate system
	int numFaces;
	int numVerts;
	string objName;

private:

	valarray<double> RefTriVertX; // current vertices of triangle faces w.r.t. local coordinate system
	valarray<double> RefTriVertY; // current vertices of triangle faces w.r.t. local coordinate system
	valarray<double> RefTriVertZ; // current vertices of triangle faces w.r.t. local coordinate system

	valarray<double> CurTriNormX; // normals at vertices
	valarray<double> CurTriNormY; // normals at vertices
	valarray<double> CurTriNormZ; // normals at vertices
	
	valarray<double> RotVelX; // rotational velocity of points
	valarray<double> RotVelY; // rotational velocity of points
	valarray<double> RotVelZ; // rotational velocity of points

	valarray<int>	 CurTriIdx; // indices into CurTriVert and CurTriNorm

	valarray<int>	VertControlPts;

	// coordinates referenced to centroid
	valarray<double> LocTriVertX; // 
	valarray<double> LocTriVertY; // 
	valarray<double> LocTriVertZ; // 
	
	// physics properties
	valarray<double> Velocity;			// x,y,z instantaneous velocity
	valarray<double> AngVel;			// x,y,z instantaneous angular velocity
	valarray<double> NetTorque;			// sum of torques at each vertex
	valarray<double> NetForce;			// sum of forces at each vertex
	valarray<double> Quarternion;		// length 4 q-vector for rotation computations
	valarray<double> RotationMatrix;	// 3x3 rotation matrix relative to initial orientation
	valarray<double> Centroid;			// the euclidean center of the object;
	valarray<double> RefCentroid;			// the euclidean center of the object;
	valarray<double> InertiaTensor;		// I, the 3x3 inertia tensor
	valarray<double> MassDistribution;	// how much mass at each vertex is there
	valarray<double> ForceDistributionX; // how much force at each vertex
	valarray<double> ForceDistributionY; // how much force at each vertex
	valarray<double> ForceDistributionZ; // how much force at each vertex
	valarray<double> TorqueDistributionX; // how much torque at each vertex
	valarray<double> TorqueDistributionY; // how much torque at each vertex
	valarray<double> TorqueDistributionZ; // how much torque at each vertex
	valarray<double> MyTemp;			// allocate one temporary array for inertia computations

	VNETexture* objTexture;
	
	double rseed, gseed, bseed; // RGB seeds for coloring the vertices
	double colorVariance;
	double mass;

	
	double radSquared;
	double elapsedTime;
	double rotSpeed;
	double minX, minY, minZ, maxX, maxY, maxZ;
	double TIMESTEP;
	double MaxVelMag;

	WorldForce* theForce;

	void ComputeCentroid();
	void ComputeInertia();
	void ComputeNetForce(); // a = F / m; do the vector-wise division and add up
	void ComputeNetTorque();
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
	
	void GetMinMaxVert(	double* minX, double* maxX, double* minY,
								double*  maxY, double* minZ, double* maxZ,
								int* minXi, int* maxXi, int* minYi, 
								int* maxYi, int* minZi, int* maxZi );
	
	void GetMinMaxVert(	);
	

	int GetNumVerts( ) { return numVerts; }
	
	double GetMass( ) { return mass; }
	double GetSpeed( );
	double GetAngSpeed( );
	double GetElapsedTime() { return elapsedTime; };
	double GetRadSquared(){return radSquared;}
	int GetNumFaces(){ return numFaces; }
	void GetCentroid(double *dx, double* dy, double* dz);
	void SetColorSeed( double r, double g, double b );
	void GetColorSeed( double* r, double* g, double* b);
	valarray<double>* GetVelocity() { return  &Velocity; }
	valarray<double>* GetCentroid() { return  &Centroid; }

	void SetSpeed( double dSpeed );
	void SetRotSpeed( double dAngVel );
	void SetVelocity( const valarray<double> &vel );
	void SetTimeStep( double dt, double vmax );

	// note: these shouldn't be used except as a user control
	void ApplyInstTorque( const valarray<double> &torque );
	void ApplyInstTorque( double dx, double dy, double dz );
	
	void AddForceAt( int vertIdx, const valarray<double> &CollideForce );
	void AddForceAllVerts( const valarray<double> &CollideForce );
	
	void UpdateVelocity( double dt );
	void UpdatePosition( double dt );
	void UpdateRotation( double dt );

	void Scale( double x, double y, double z );

	// helper subroutine
	void RotateLocal( );
	void ComputeTorqueDistribution( );
	void ComputeRotationalVelocity( );
	
	// BELOW ARE DEPRECATED !!!!
	int TranslateTo( double dx, double dy, double dz );
	int TranslateBy( double dx, double dy, double dz );
	void SetVelocityProfile(double xval, double yval, double zval, int iCode );
};







#endif

