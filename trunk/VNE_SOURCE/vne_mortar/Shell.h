#ifndef SHELL
#define SHELL

#include "../vne_engine/PhysObject.h"

class Shell : public PhysObject
{
	
private:
	bool bIsExploding;

public:
	Shell();
	Shell(const Shell &);
	Shell( ObjParams params );
	void Destruct() { Explode(); }
	bool IsDestructing() { return bIsExploding; }
	void Explode();
	void GrabCamera();
	void UpdateSelf();

};

#endif