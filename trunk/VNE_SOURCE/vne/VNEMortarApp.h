#ifndef VNEMortarAPP
#define VNEMortarAPP

#include "VNEWorld.h"
#include "VNEObject.h"
#include "CameraControl.h"

class VNEMortarApp
{
private:
	VNEWorld* world;
	VNEObject* DemoObj;
	CameraControl* camera;
	int counter;
	int iControlObjIdx;

public:
	VNEMortarApp();
	~VNEMortarApp();

	void KeyboardCallback(unsigned char key, int x, int y);
	void MouseCallback();
	void IdleCallback();
	void DisplayCallback();
	void ResizeCallback(int w, int h);

};




#endif