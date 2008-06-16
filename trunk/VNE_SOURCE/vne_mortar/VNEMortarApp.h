#ifndef VNEMortarAPP
#define VNEMortarAPP

#include "../vne_engine/CameraControl.h"
#include "../vne_engine/ObjectController.h"
#include "Mortar.h"
#include "Shell.h"

class VNEMortarApp
{
private:
	CameraControl* camera;
	ObjectController* objControl;
	int iControlObjIdx;
	Mortar* mortar1;
	Mortar* mortar2;
	Mortar* focusMortar;
	double xmin, ymin, xmax, ymax, zmin, zmax;
	VNETexture* floorTex;
	VNETexture* backTex;
	void DrawWalls();

public:
	VNEMortarApp();
	~VNEMortarApp();

	void Init();
	void Reset();
	void KeyboardCallback(unsigned char key, int x, int y);
	void MouseCallback(int button, int state, int x, int y);
	void IdleCallback();
	void DisplayCallback();
	void ResizeCallback(int w, int h);

};




#endif