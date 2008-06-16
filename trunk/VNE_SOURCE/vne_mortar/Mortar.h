#ifndef MORTAR
#define MORTAR

#include "../vne_engine/PhysObject.h"
#include "Shell.h"

class Mortar : public PhysObject
{
private:
	bool bIsFiring;
	bool bIsHit;
	Shell* myShell;
	double power;
	double dHitDist;
	valarray<double> direction;
	valarray<double> refdirection;

public:
	Mortar();
	Mortar(const Mortar &);
	Shell* GetSpawnedObject();
	void UpdateSelf();
	bool IsSpawning() {return bIsFiring;}
	bool IsDestructing() {return bIsHit; }
	void StopSpawn();
	void Fire(); // mortar fires a shell
	void RegisterHit() {bIsHit = true;} // mortar is hit by a shell
	void Destruct(); 
	void HandleDestruct( Object* other );
	void GrabCamera(CameraControl* camera); // view the mortar up close
	void GrabCamera(CameraControl* camera, PhysObject* target);
	void SetPower( double d ) { power = d;  }
	void TiltBy( const valarray<double> &delta );
	void TiltTo( const valarray<double> &vec );
	double GetPower() {return power; }
	valarray<double> GetDir() { return direction; }

};

#endif