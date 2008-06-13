// a physical object must time-step itself carefully
// keep track of centroid, inertia, and actual
// vertex locations

#ifndef PHYSOBJECT
#define PHYSOBJECT

#include "Object.h"
#include <valarray>

struct ObjParams {
	valarray<double> InitPosition;
	valarray<double> InitVelocity;
	vector<string> input;
};

class PhysObject : public Object
{
protected:
	static double step; // default timestep
	double massTotal;

	valarray<double> Inertia;
	valarray<double> Centroid;
	valarray<double> Velocity;
	valarray<double> AngVel;
	valarray<double> Rotation;
	valarray<double> Quarternion;
	valarray<double> MassDistribution;
	valarray<double> ForceDistributionX;
	valarray<double> ForceDistributionY;
	valarray<double> ForceDistributionZ;
	valarray<double> TorqueDistributionX;
	valarray<double> TorqueDistributionY;
	valarray<double> TorqueDistributionZ;
	valarray<double> RotVelX;			// rotational velocity of points
	valarray<double> RotVelY;			
	valarray<double> RotVelZ;			
	valarray<double> NetTorque;			// sum of torques at each vertex
	valarray<double> NetForce;			// sum of forces at each vertex

	valarray<double> CurVertX; 
	valarray<double> CurVertY; 
	valarray<double> CurVertZ;
	valarray<double> LocVertX; 
	valarray<double> LocVertY; 
	valarray<double> LocVertZ; 
	valarray<double> RefVertX; // current vertices of triangle faces w.r.t. local coordinate system
	valarray<double> RefVertY; // current vertices of triangle faces w.r.t. local coordinate system
	valarray<double> RefVertZ; // current vertices of triangle faces w.r.t. local coordinate system

	valarray<double> MyTemp;   // this object's temporary numVerts-sized array
	valarray<int>	VertControlPts; // holds indices of min/max x,y,z vertices

	valarray<double> CurNormX;	// normals at vertices
	valarray<double> CurNormY;	// normals at vertices
	valarray<double> CurNormZ;	// normals at vertices
	valarray<int>	 CurIdx;	// indices into Verts & Norms
	
	int numFaces;
	int numVerts;

	virtual void ComputeInertia();
	virtual void ComputeTorqueDistribution();
	virtual void ComputeRotationMatrix();
	virtual void ComputeRotationMatrix( const valarray<double> &ref, const valarray<double> &newdir );
	virtual void ComputeRotationalVelocity();
	virtual void ComputeCentroid();
	virtual void ComputeNetTorque();
	virtual void ComputeNetForce();
	virtual void UpdateRotation();
	virtual void UpdateTotals();
	virtual void UpdateAngVel(double dt);
	virtual void EulerTimeStep(double dt);

	virtual void ProcessTriDataFiles( const vector<string> filenames );
	virtual void SyncDrawObj();


public:
	// constructors
	PhysObject();
	PhysObject(const PhysObject &);
	PhysObject(const vector<string> &filenames);
	
	virtual void UpdateSelf() {};
	virtual void InitFromFile(const vector<string> &filenames);
	virtual PhysObject* GetSpawnedObject() { return NULL; }
	virtual bool IsSpawning() { return false; }
	virtual int GetNumVerts() { return numVerts; }
	virtual int GetNumFaces() { return numFaces; }
	virtual void TimeStep( double dt = step ) {}
	virtual void GetMinMaxVert( );
	virtual void AddForceAllVerts( const valarray<double> &Force );
	virtual void AddForceAllVerts( double fx, double fy, double fz );
	virtual void AddForceAt( int vertIdx, const valarray<double> &Force );
	virtual void AddForceAt( int vertIdx, double fx, double fy, double fz );
	virtual void SetVelocity( double vx, double vy, double vz );
	virtual void TranslateTo(double dx, double dy, double dz);
	virtual void TranslateBy(double dx, double dy, double dz);
	virtual void AccelerateBy(double dx, double dy, double dz);
	virtual void ClearForce();
};

#endif