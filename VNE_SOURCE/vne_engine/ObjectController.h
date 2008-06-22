// object controller
// has lists of Rigid Objects, Surface Objects
// Operates on the objects telling them to draw themselves

#ifndef OBJECTCNTRL
#define OBJECTCNTRL

#include "PhysObject.h"
#include "Solver.h"
#include "CameraControl.h"

class ObjectController
{
private:
	vector<PhysObject*> physObjs;
	//vector<SurfaceObject*> surfObjs;
	//Solver* solver;
	CameraControl* camera;
	bool bIsCollision;
public:
	ObjectController();
	~ObjectController();
	PhysObject* GetPhysObj( unsigned int idx );
	void AddPhysObj( PhysObject* obj );
	void DeletePhysObj( int idx );
	void Update();
	void ApplyGravity(double g);
	int NumPhysObjs() { return physObjs.size(); }
	CameraControl* GetCamera();
	bool IsCollision();
	void GetCollisionID( int* idx1, int* idx2);

};

#endif