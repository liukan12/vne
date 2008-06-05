#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "VNEApp.h"
#include "VNEWorld.h"
#include <time.h>
#include "WorldForce.h"
#include "SOIL.h"
#include "VNETexture.h"
#include <numeric>
#include <valarray>

using namespace std;

#define PERFORMANCE_TEST_NO

double minTimeStep;
#define DRAWTIMESTEP 0.01

VNEWorld::VNEWorld()
{

	spatialResolution = .01;
	double maxVmag = 30;
	minTimeStep = 0.5 * spatialResolution / maxVmag;
	// time step minimum to prevent things from "flying through" eachother during one time step
	// like "CFL Condition"

	string faces, faces2, faces3, faces4, faces5;
	string verts, verts2, verts3, verts4, verts5;
	string norms, norms2, norms3, norms4, norms5;
#define PROFILE_NO
#ifdef PROFILE
	faces =  "faces4.dat";
	verts =  "verts4.dat";
	norms =  "norms4.dat";
	faces2 = "faces5.dat";
	verts2 = "verts5.dat";
	norms2 = "norms5.dat";
	faces3 = "faces6.dat";
	verts3 = "verts6.dat";
	norms3 = "norms6.dat";
	faces4 = "brainfaces.dat";
	verts4 = "brainverts.dat";
	norms4 = "brainnorms.dat";
#elif _DEBUG // debug looks in relative path (vne data files)
	faces = "..\\vne_data\\faces7.dat";
	verts = "..\\vne_data\\verts7.dat";
	norms = "..\\vne_data\\norms7.dat";
	faces2 = "..\\vne_data\\faces5.dat";
	verts2 = "..\\vne_data\\verts5.dat";
	norms2 = "..\\vne_data\\norms5.dat";
	faces3 = "..\\vne_data\\faces6.dat";
	verts3 = "..\\vne_data\\verts6.dat";
	norms3 = "..\\vne_data\\norms6.dat";
	faces4 = "..\\vne_data\\brainfaces.dat";
	verts4 = "..\\vne_data\\brainverts.dat";
	norms4 = "..\\vne_data\\brainnorms.dat";
	faces5 = "..\\vne_data\\faces7.dat";
	verts5 = "..\\vne_data\\verts7.dat";
	norms5 = "..\\vne_data\\norms7.dat";
#else // release build looks in the same directory as .exe for data files
	faces = ".\\data\\faces4.dat";
	verts = ".\\data\\verts4.dat";
	norms = ".\\data\\norms4.dat";
	faces2= ".\\data\\faces5.dat";
	verts2= ".\\data\\verts5.dat";
	norms2= ".\\data\\norms5.dat";
	faces3= ".\\data\\faces6.dat";
	verts3= ".\\data\\verts6.dat";
	norms3= ".\\data\\norms6.dat";
	faces4 = ".\\data\\brainfaces.dat";
	verts4 = ".\\data\\brainverts.dat";
	norms4 = ".\\data\\brainnorms.dat";
#endif

	// TODO: we need OBJECT FILES that define all file names and properties
	VNEObject* Obj1 = new VNEObject( "object 1", faces, verts, norms);
	VNEObject* Obj2 = new VNEObject( "object 2", faces2, verts2, norms2);
	VNEObject* Obj3 = new VNEObject( "object 3", faces3, verts3, norms3);
	//VNEObject* Obj4 = new VNEObject( "object 4", faces4, verts4, norms4); // no norms for the brain 
#ifdef PROFILE
	Obj1->setTexture("obj1.png");
	Obj2->setTexture("obj2.png");
	Obj3->setTexture("obj3.png");
#elif _DEBUG
	Obj1->setTexture("..\\vne_data\\obj1.png");
	Obj2->setTexture("..\\vne_data\\obj2.png");
	Obj3->setTexture("..\\vne_data\\obj3.png");
#else
	Obj1->setTexture(".\\data\\obj1.png");
	Obj2->setTexture(".\\data\\obj2.png");
	Obj3->setTexture(".\\data\\obj3.png");
#endif
	
	Obj1->SetVelocityProfile( 0.5, 0.0, 0.5, 0 );
	Obj2->SetVelocityProfile( -0.5, 0.5, 0.5, 0 );
	Obj3->SetVelocityProfile( -0.5, 0.5, 0.5, 0 );
		
	Obj1->SetColorSeed(-1.5,-2.0,1.0);
	Obj2->SetColorSeed(1.0,0.0,0.5);
	Obj3->SetColorSeed(0.5,0.5,0.0);
	
	Obj1->TranslateTo(0.0,2.0,0.0);
	Obj2->TranslateTo(-2.0,-2.0,-2.0);
	Obj3->TranslateTo(0.0,1.0,0.0);
	
	/*Obj1->Scale(2.0,2.0,2.0);
	Obj2->Scale(2.0,2.0,2.0);
	Obj3->Scale(4.0,3.0,3.0);*/

	Obj1->SetTimeStep( minTimeStep, maxVmag );
	Obj2->SetTimeStep( minTimeStep, maxVmag );
	Obj3->SetTimeStep( minTimeStep, maxVmag );

	this->ObjList = new VNEObjList( Obj1 );
	this->ObjList->AddObj(Obj2);
	this->ObjList->AddObj(Obj3);
	//this->ObjList->AddObj(Obj4);

#ifdef PERFORMANCE_TEST
	VNEObject* Obj5 = new VNEObject( "object 5", faces5, verts5, norms5); // object with 10K vertices
	Obj5->SetVelocityProfile( 0.0, -0.5, -0.5, 0 );
	Obj5->TranslateTo(2.0,-3.0,-1.0);
	Obj5->SetColorSeed(0.25,0.15,1.0);
	this->ObjList->AddObj(Obj5);
#endif


	this->ObjList->PrintAll();

	
	//theForce->VortexOff();

	xmin = -50;
	xmax = 50;
	ymin = -50;
	ymax = 50;
	zmin = -5;
	zmax = 15;
	
	clock1 = clock();
	elapsedTime = 0.0;
	glShadeModel( GL_SMOOTH );
	LightsOff();
	
#ifdef PROFILE
	myTex=new VNETexture("cat.jpg");
#elif _DEBUG
	myTex1=new VNETexture("..\\vne_data\\stars.jpg");
	myTex2=new VNETexture("..\\vne_data\\ground.jpg");
#else
	myTex=new VNETexture(".\\data\\cat.jpg");
#endif

}

void VNEWorld::LightsOn()
{
	bLightsOn = true;
	GLfloat light_position[] = {0.0,0.0,4.0,0.0};
	GLfloat light1_specular[] = {1.0,1.0,1.0,0.0};
	GLfloat light1_ambient[] = {1.0, 0.1, 0.5,0.0};
	GLfloat light1_diffuse[] = {1.0, 1.0, 1.0,0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse);
	GLfloat light2_position[] = {3.0,0.0,0.0,0.0};
	GLfloat light2_specular[] = {1.0,1.0,1.0,0.0};
	GLfloat light2_ambient[] = {0.5, 0.1, 1.0,0.0};
	GLfloat light2_diffuse[] = {1.0, 0.0, 1.0,0.0};
	glLightfv(GL_LIGHT1, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light2_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_diffuse);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}
void VNEWorld::LightsOff()
{
	bLightsOn = false;
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);
}

void VNEWorld::EnableForce( int iNum )
{
	switch( iNum )
	{
	case 0:
	//	this->theForce->VortexOn();
		break;
	default:
		cout<<"Invalid number in EnableForce()!\n";
		break;
	}
}
void VNEWorld::DisableForce( int iNum )
{
	switch( iNum )
	{
	case 0:
	//	this->theForce->VortexOff();
		break;
	default:
		cout<<"Invalid number in DisableForce()!\n";
		break;
	}
}
void VNEWorld::Collide(VNEObject* obj1, VNEObject* obj2)
{
	double m1 = obj1->GetMass();
	double m2 = obj2->GetMass();
	double totalM = m1+m2;




}

valarray<double> diffx;
valarray<double> diffy;
valarray<double> diffz;

void VNEWorld::CheckCollisions()
{
	int i,n;
	VNEObject* obj1;
	VNEObject* obj2;
	
	for(i=0; i < this->ObjList->Length(); i++)
	{
		obj1 = this->ObjList->GetObjectAt(i);
		for(n=i+1; n < this->ObjList->Length(); n++)
		{
			obj2 = this->ObjList->GetObjectAt(n);
			
			double dist;
			double minDist = 1e3;
			for( int i = 0; i < obj1->numVerts; i++ ) {
				for( int j = 0; j < obj2->numVerts; j++ ) {
					dist = pow(obj1->CurTriVertX[i]-obj2->CurTriVertX[j],2)+pow(obj1->CurTriVertY[i]-obj2->CurTriVertY[j],2)+pow(obj1->CurTriVertZ[i]-obj2->CurTriVertZ[j],2);
					if( dist < minDist )
						minDist = dist;
				}
			}

			if( spatialResolution > minDist )
			{
				this->Collide( obj1, obj2);
			}
		}
	}
}

int VNEWorld::TimeStep()
{
	for( int i = 0; i < 20; i++ ) {
	this->CheckCollisions();   // THIS NEEDS HUGE OVERHAUL
	this->ObjList->TimeStepAll();
	}
	
	return 0;
}

void VNEWorld::DrawWalls()
{
	for( int i = 0; i < 2; i++ )
	{
		glPushMatrix();
		switch(i)
		{ // draw five walls, leave one open area for the camera's default position
		case 0: 
			//glRotatef(90.0,1.0,0.0,0.0);
			myTex1->bindTexture();
			break;
		case 1:
			glRotatef(-90,1.0,0.0,0.0);
			myTex2->bindTexture();
			break;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		
		glBegin(GL_QUADS);
			// back wall
			glTexCoord2f(0.0, 0.0);
			glVertex3f(xmin, ymin, zmin+1e-3 );
			glTexCoord2f(0.0, 3.0);
			glVertex3f(xmin,ymax,zmin+1e-3);
			glTexCoord2f(3.0,3.0);
			glVertex3f(xmax,ymax,zmin+1e-3);
			glTexCoord2f(3.0,0.0);
			glVertex3f(xmax, ymin, zmin+1e-3 );

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	glFlush();   // doesn't seem necessary
}

int VNEWorld::Redraw()
{
	int result = 0;

	//if( this->theForce->WallsAreOn() )
		DrawWalls();

	result = this->ObjList->DrawAll();

	return result;
}

void VNEWorld::PrintWorldState()
{
	// output in console the state of all objects, forces, lighting, etc
	cout<<"The state of the world is: \n";

}

double VNEWorld::Getxmax()
{ return xmax; }

double VNEWorld::Getymax()
{ return ymax; }

double VNEWorld::Getzmax()
{ return zmax; }

double VNEWorld::Getxmin()
{ return xmin; }

double VNEWorld::Getymin()
{ return ymin; }

double VNEWorld::Getzmin()
{ return zmin; }
