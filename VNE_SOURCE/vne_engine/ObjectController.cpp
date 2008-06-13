#include "ObjectController.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

ObjectController::ObjectController()
{
	this->physObjs = vector<PhysObject*>(0);
}
PhysObject* ObjectController::GetPhysObj( unsigned int idx )
{
	if( idx >= physObjs.size() ) {
		cout<<"error, entered index of object is out of bounds!\n";
		return physObjs[physObjs.size()-1];
	}
	return physObjs[idx];
}

void ObjectController::AddPhysObj( PhysObject* obj )
{
	physObjs.push_back(obj);
}

void ObjectController::DeletePhysObj( int idx )
{
	
}

void ObjectController::ApplyGravity(double g)
{
	valarray<double> gravity(3);
	gravity[1] = -g;
	for( unsigned int i = 0; i < physObjs.size(); i++ ) {
		physObjs[i]->AddForceAllVerts(gravity);
	}
}

void ObjectController::Update()
{
	for( unsigned int i = 0; i < physObjs.size(); i++ ) {
		if( this->physObjs[i]->IsSpawning() ) {
			PhysObject* newObj = physObjs[i]->GetSpawnedObject();
			this->physObjs.push_back(newObj);
			physObjs[i]->StopSpawn();
		}
	}
	for( unsigned int i = 0; i < physObjs.size(); i++ ) {
		physObjs[i]->UpdateSelf();
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for( unsigned int i = 0; i < physObjs.size(); i++ ) {
		physObjs[i]->GetDrawObj()->DrawSelf();
	}
	glColor3f(1.0,1.0,1.0);
}

CameraControl* ObjectController::GetCamera()
{
	return NULL;
}

bool ObjectController::IsCollision()
{
	return false;
}

void ObjectController::GetCollisionID( int* idx1, int* idx2)
{

}