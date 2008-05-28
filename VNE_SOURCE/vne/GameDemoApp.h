#ifndef GAMEDEMOAPP
#define GAMEDEMOAPP

#include "VNEWorld.h"
#include "VNEObject.h"
#include "CameraControl.h"

class GameDemoApp
{
private:
	VNEWorld* world;
	VNEObject* DemoObj;
	CameraControl* camera;
	int counter;
	int iControlObjIdx;

public:
	GameDemoApp();
	~GameDemoApp();

	void KeyboardCallback(unsigned char key, int x, int y);
	void MouseCallback();
	void IdleCallback();
	void DisplayCallback();
	void ResizeCallback(int w, int h);

};




#endif