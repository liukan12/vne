#ifndef OBSTACLE
#define OBSTACLE

#include "../vne_engine/PhysObject.h"
#include "Globals.h"

class Obstacle : public PhysObject
{
private:
	void FloorCollide();

public:
	Obstacle();
	Obstacle(const Obstacle &);
	void UpdateSelf();
	void GrabCamera(CameraControl* camera); // view the mortar up close
	void GrabCamera(CameraControl* camera, PhysObject* target);
	bool isStatic() { return false; }

};

#endif