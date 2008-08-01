#ifndef VNESTEREO
#define VNESTEREO

#include "../vne_engine/CameraControl.h"
#include "../vne_engine/ObjectController.h"
#include "../vne_engine/PhysObject.h"

class VNEStereoApp
{
private:
	CameraControl* camera;
	ObjectController* objControl;
	PhysObject* obj1;

	// camera positions
	vector<float> xcpos;
	vector<float> ycpos;
	vector<float> zcpos;

	// target positions
	vector<float> xtpos;
	vector<float> ytpos;
	vector<float> ztpos;

	int iControlObjIdx;
	int camidx;
	double xmin, ymin, xmax, ymax, zmin, zmax;
	
public:
	VNEStereoApp();
	~VNEStereoApp();

	void Init();
	void Reset();
	void KeyboardCallback(unsigned char key, int x, int y);
	void MouseCallback(int button, int state, int x, int y);
	void IdleCallback();
	void DisplayCallback();
	void ResizeCallback(int w, int h);

};




#endif