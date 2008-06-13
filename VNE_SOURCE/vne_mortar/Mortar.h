#ifndef MORTAR
#define MORTAR

#include "../vne_engine/PhysObject.h"
#include "Shell.h"

class Mortar : public PhysObject
{
private:
	bool bIsFiring;
	Shell* myShell;
	double power;
	valarray<double> direction;
	valarray<double> refdirection;

public:
	Mortar();
	Mortar(const Mortar &);
	Shell* GetSpawnedObject();
	void UpdateSelf();
	bool IsSpawning() {return bIsFiring;}
	void StopSpawn();
	void Fire(); // mortar fires a shell
	void Destruct(); // mortar is hit by a shell
	void GrabCamera(CameraControl* camera); // view the mortar up close
	void SetPower( double d ) { power = d; }
	void TiltBy( const valarray<double> &delta );
	void TiltTo( const valarray<double> &vec );
	double GetPower() {return power; }

};

#endif