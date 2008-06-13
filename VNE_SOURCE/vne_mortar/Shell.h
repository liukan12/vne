#ifndef SHELL
#define SHELL

#include "../vne_engine/PhysObject.h"

class Shell : public PhysObject
{
private:

public:
	Shell();
	Shell(const Shell &);
	Shell( ObjParams params );
	void Explode();
	void GrabCamera();
	void UpdateSelf();

};

#endif