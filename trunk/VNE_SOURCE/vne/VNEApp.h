#ifndef VNEAPP
#define VNEAPP

#include "VNEWorld.h"
#include "VNEObject.h"

class VNEApp
{
private:
	VNEWorld* world;
	VNEObject* DemoObj;
	int counter;

public:
	VNEApp();
	~VNEApp();

	//static VNEWorld* world;
	void KeyboardCallback(unsigned char key, int x, int y);
	void MouseCallback();
	void IdleCallback();
	void DisplayCallback();
	void ResizeCallback(int w, int h);

};




#endif