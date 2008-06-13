#include "PhysObject.h"

class Solver
{
protected:
	vector<PhysObject*> physObjs;
	
	virtual void CheckCollisions() {};
	virtual void TimeStep() {};

};