#include "Mortar.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

Mortar::Mortar()
{
	bIsFiring = false;
	bIsHit = false;
	this->direction = valarray<double>(3);
	this->direction[1] = 1.0; // initially point up
	this->refdirection = valarray<double>(3);
	this->refdirection[1] = 1.0; // initially point up
	power = 4.0;
	dHitDist = 1.0;
}

Mortar::Mortar(const Mortar &)
{

}

void Mortar::TiltBy( const valarray<double> &delta )
{
	valarray<double> newdir(3);
	newdir = delta + direction;
	valarray<double> temp(3);
	temp = newdir*newdir;
	newdir = newdir / sqrt(temp.sum());
	this->ComputeRotationMatrix( direction, newdir );
	direction = newdir;

}

void Mortar::TiltTo( const valarray<double> &vec )
{
	valarray<double> newdir(3);
	valarray<double> temp(3);
	temp = vec*vec;
	newdir = vec / sqrt(temp.sum());
	
	this->ComputeRotationMatrix( refdirection, newdir );
	direction = newdir;

}

Shell* Mortar::GetSpawnedObject()
{
	ObjParams params;
	params.InitPosition = valarray<double>(3);
	params.InitPosition = Centroid;
	params.InitVelocity = valarray<double>(3);
	params.InitVelocity = power * direction;
	params.input = vector<string>(4);
	params.input[3] = "tri";
	params.input[0] = "..\\vne_data\\faces7.dat";//facesshell.dat"; 
	params.input[1] = "..\\vne_data\\norms7.dat";//normsshell.dat";
	params.input[2] = "..\\vne_data\\verts7.dat";//vertsshell.dat";
	return new Shell(params);
}

void Mortar::GrabCamera(CameraControl* camera)
{
	// do this SLOWLY with animation!
	camera->TranslateTo( Centroid[0]-direction[0]*3, 3.0, Centroid[2]-direction[2]*3 );
	camera->PointAt( 0, 0, 0 );
}

void Mortar::GrabCamera(CameraControl* camera, PhysObject* target)
{
	// do this SLOWLY with animation!
	valarray<double> temp = target->GetCentroid();
	valarray<double> toMe = Centroid - temp;
	toMe = toMe / sqrt( (toMe*toMe).sum() );

	camera->TranslateTo( Centroid[0] + 5*toMe[0], 3.0 , Centroid[2] + 5*toMe[2] );
	
	camera->PointAt( temp[0], temp[1], temp[2] );
}

void Mortar::UpdateSelf()
{
	this->UpdateRotation();
	this->SyncDrawObj();
}

void Mortar::StopSpawn()
{
	this->bIsFiring = false;
}

void Mortar::Fire()
{
	this->bIsFiring = true;
}

void Mortar::Destruct()
{
	cout<<this->objName<<" is hit!\n";
}

void Mortar::HandleDestruct( Object* other )
{
	Shell* shell = dynamic_cast<Shell*>(other);
	if( !shell ) // something other than a shell
		return;
	else {
		valarray<double> loc = shell->GetCentroid();
		loc = Centroid - loc;
		loc[1] = 0.0;
		double dist = sqrt((loc*loc).sum());
		if( dist < dHitDist ) {
			this->bIsHit = true;
		}
	}
}

