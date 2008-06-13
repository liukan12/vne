#include "Mortar.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

Mortar::Mortar()
{
	bIsFiring = false;
	this->direction = valarray<double>(3);
	this->direction[1] = 1.0; // initially point up
	this->refdirection = valarray<double>(3);
	this->refdirection[1] = 1.0; // initially point up
	power = 10.0;
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
	params.InitVelocity = power * direction; // TODO: change this!
	params.input = vector<string>(4);
	params.input[3] = "tri";
	params.input[0] = "..\\vne_data\\facesshell.dat"; 
	params.input[1] = "..\\vne_data\\normsshell.dat";
	params.input[2] = "..\\vne_data\\vertsshell.dat";
	return new Shell(params);
}

void Mortar::GrabCamera(CameraControl* camera)
{
	// do this SLOWLY with animation!
	camera->TranslateTo( 0,2.0,0 );
	camera->PointAt( Centroid[0], Centroid[1], Centroid[2] );
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

