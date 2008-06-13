#ifndef OBJECT
#define OBJECT

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include "DrawObject.h"
#include "CameraControl.h"

using namespace std;

class Object
{
private:

protected:
	DrawObject* drawObject;
	string objName;

public:
	Object() {};
	Object(const Object & ) {};
	Object(const vector<string> &filenames) {};
	virtual void SetName( const string &name ) { objName = name; }
	virtual string Name() { return objName; }
	virtual void UpdateSelf() {};
	virtual void InitFromFile(const vector<string> &filenames){};
	virtual Object* GetSpawnedObject() {return NULL;}
	virtual bool IsSpawning() {return false;}
	virtual void StopSpawn() {}
	virtual void GrabCamera(CameraControl* camera) {}
	void Error() {	cout<<"Press any key to quit.";	char in; cin>>in; exit(1); }
	virtual DrawObject* GetDrawObj() { return drawObject; }
};

#endif