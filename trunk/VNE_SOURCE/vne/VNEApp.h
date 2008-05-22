#ifndef VNEAPP
#define VNEAPP

#include "VNEWorld.h"
#include "VNEObject.h"
#include "CameraControl.h"

class VNEApp
{
private:
	VNEWorld* world;
	VNEObject* DemoObj;
	CameraControl* camera;
	int counter;

public:
	VNEApp();
	~VNEApp();

	void KeyboardCallback(unsigned char key, int x, int y);
	void MouseCallback();
	void IdleCallback();
	void DisplayCallback();
	void ResizeCallback(int w, int h);

};




#endif