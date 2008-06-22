#include "Obstacle.h"

Obstacle::Obstacle()
{


}

void Obstacle::UpdateSelf()
{
	valarray<double> gravity(3);
	gravity[1] = GRAV/numVerts;
	this->GetMinMaxVert();
	this->FloorCollide();
	this->AddForceAllVerts(gravity);
	this->ProcessCollisions();
	this->UpdateTotals();
	this->EulerTimeStep( TIMESTEP );
	this->SyncDrawObj();
	

	

}

void Obstacle::FloorCollide()
{
	double miny = CurVertY[this->VertControlPts[1]];
	double totalVy = RotVelY[this->VertControlPts[1]] + Velocity[1];
	if( totalVy < 0.0 && miny <= 0 ) {

		// 1. first: time step him to find the precise point of collision
	double tcross = 0.0;
	double distThresh = 1e-3;
	double errorMax = 1e-3;
	double dist = abs(CurVertY[this->VertControlPts[1]]);
	double t1lo = -this->timeStep;
	double t1hi = 0.0;
	double tshift1 = 0.5*(t1lo + t1hi);
	
	double prevY = miny + tshift1 * totalVy;
	// find a backward step that definately takes us out of collision
	int its = 0;
	while( (prevY < 0) && its < 20 ) {
		t1lo *= 2;
		prevY = miny + tshift1 * totalVy;
		tshift1 = 0.5*(t1lo + t1hi);
		its++;
	}

	int i = 0;
	int iMax = 30;
	while( i <= iMax && abs(dist - distThresh) > errorMax ) {
	// ok at the current situation we have a collision, but not at previous time step.
	// subdivide time (bisection) until the distance is "close" to distThresh. this is the collision time.
	
		tshift1 = 0.5*(t1lo + t1hi);

		miny = miny + totalVy*tshift1;
		dist = abs(miny);
		if( abs(dist - distThresh) <= errorMax )
			break;
		if( dist > distThresh ) { // we moved too far back. lo times need to increase
			miny = miny - totalVy*tshift1;
			t1lo = tshift1;
		}
		else { // not far enough. decrease hi time.
			miny = miny - totalVy*tshift1;
			t1hi = tshift1;
		}
		i++;
	}
		this->TranslateBy( tshift1*Velocity[0], tshift1*totalVy, tshift1*Velocity[2] );
		

		// 3. two things need to get changed: one, the linear velocity normal
		// to wall needs to be reversed. second, the rotational velocity normal 
		// to wall needs reversal while not generating a net force
		valarray<double> floorBounce(3);
		floorBounce[1] = -Velocity[1] / TIMESTEP * 1.6;
		valarray<double> floorBounceRot(3);
		valarray<double> floorBounceRotBalance(3);
		floorBounceRot[1] = -RotVelY[VertControlPts[1]] / TIMESTEP * 1.6;
		floorBounceRotBalance[1] = -floorBounceRot[1] / numVerts;
		this->AddForceAt( VertControlPts[1], floorBounce );
		this->AddForceAt( VertControlPts[1], floorBounceRot );
		valarray<double> gravity(3);
		gravity[1] = GRAV/numVerts;
		this->AddForceAt( VertControlPts[1], -gravity );
		this->AddForceAllVerts( floorBounceRotBalance );
		this->GetMinMaxVert();
#ifdef _DEBUG
		this->UpdateTotals();
		if( abs( NetForce[1] ) > 200.0 )
			int breakhere = 1;
		if( abs(Centroid[1]) > 100 )
			int ibreakhere = 1;
#endif
	}
	
	
	
	
}


void Obstacle::GrabCamera(CameraControl* camera)
{

}
void Obstacle::GrabCamera(CameraControl* camera, PhysObject* target)
{

}
