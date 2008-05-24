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
#include "MathUtils.h"
using std::cout;

#define CheckerTexSize 64
static GLubyte CheckerTex[CheckerTexSize][CheckerTexSize][4];
static GLuint texName;

VNEWorld::VNEWorld()
{
	string faces, faces2, faces3;
	string verts, verts2, verts3;
	string norms, norms2, norms3;
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
#elif _DEBUG // debug looks in relative path (vne data files)
	faces = "..\\vne_data\\faces4.dat";
	verts = "..\\vne_data\\verts4.dat";
	norms = "..\\vne_data\\norms4.dat";
	faces2 = "..\\vne_data\\faces5.dat";
	verts2 = "..\\vne_data\\verts5.dat";
	norms2 = "..\\vne_data\\norms5.dat";
	faces3 = "..\\vne_data\\faces6.dat";
	verts3 = "..\\vne_data\\verts6.dat";
	norms3 = "..\\vne_data\\norms6.dat";
#else // release build looks in the same directory as .exe for data files
	faces = "faces4.dat";
	verts = "verts4.dat";
	norms = "norms4.dat";
	faces2= "faces5.dat";
	verts2= "verts5.dat";
	norms2 = "norms5.dat";
	faces3= "faces6.dat";
	verts3= "verts6.dat";
	norms3= "norms6.dat";
#endif

	// TODO: we need OBJECT FILES that define all file names and properties
	VNEObject* Obj1 = new VNEObject( "object 1", faces, verts, norms);
	VNEObject* Obj2 = new VNEObject( "object 2", faces2, verts2, norms2);
	VNEObject* Obj3 = new VNEObject( "object 3", faces3, verts3, norms3);
	
	Obj1->SetVelocityProfile( 0.5, 0.5, 0.0, 0 );
	Obj2->SetVelocityProfile( -0.5, 0.5, 0.5, 0 );
	Obj3->SetVelocityProfile( 0.5, -0.5, -0.5, 0 );
	
	Obj1->SetColorSeed(0.5,0.0,1.0);
	Obj2->SetColorSeed(1.0,0.0,0.5);
	Obj3->SetColorSeed(0.5,0.5,0.0);

	this->ObjList = new VNEObjList( Obj1 );
	this->ObjList->AddObj(Obj2);
	this->ObjList->AddObj(Obj3);

	this->ObjList->PrintAll();

	this->theForce = new WorldForce();
	theForce->VortexOff();

	xmin = -5;
	xmax = 5;
	ymin = -5;
	ymax = 5;
	zmin = -5;
	zmax = 15;
	clock1 = clock();
	elapsedTime = 0.0;
	glShadeModel( GL_SMOOTH );
	LightsOff();

	TexturesOn();
}

void VNEWorld::TexturesOn()
{
	int i,j,c;
	for( i = 0; i < CheckerTexSize; i++ ) {
		for( j = 0; j < CheckerTexSize; j++) {
			c = ((((i&0x8)==0)^((j&0x8))==0))*255;
				CheckerTex[i][j][0] = (GLubyte) c/255*2*i;
				CheckerTex[i][j][1] = (GLubyte) c/255*4*j;
				CheckerTex[i][j][2] = (GLubyte) c/255*4*i;
				CheckerTex[i][j][3] = (GLubyte) 255;
		}
	}

	glEnable( GL_TEXTURE_2D );
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CheckerTexSize, CheckerTexSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, CheckerTex );
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
		this->theForce->VortexOn();
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
		this->theForce->VortexOff();
	default:
		cout<<"Invalid number in DisableForce()!\n";
		break;
	}
}

int VNEWorld::TimeStep()
{
	int result = 0;

	// for now do nothing; later, following a script or physics sim
	// the objects will update position / rotation

	clock2 = clock();
	double dt = ( (double) (clock2 - clock1) ) / CLOCKS_PER_SEC ;
	elapsedTime += dt;
	clock1 = clock();

	//DemoObj->IncrementTime(); // use constant time flow, otherwise user interaction
	// causes errors (too much delay)
	this->ObjList->AccelAll( this->theForce );
	result = this->ObjList->TimeStepAll();

	return result;
}

void VNEWorld::DrawWalls()
{
	for( int i = 0; i < 5; i++ )
	{
		glPushMatrix();
		switch(i)
		{ // draw five walls, leave one open area for the camera's default position
		case 0: 
			glRotatef(90.0,0.0,1.0,0.0);
			break;
		case 1:
			glRotatef(-90,0.0,1.0,0.0);
			break;
		case 2:
			break;
		case 3:
			glRotatef(90,1.0,0.0,0.0);
			break;
		case 4:
			glRotatef(-90,1.0,0.0,0.0);
			break;
		}
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
		glBindTexture(GL_TEXTURE_2D, texName );

		glBegin(GL_QUADS);
			// back wall
			glTexCoord2f(0.0, 0.0);
			glVertex3f(xmin, ymin, zmin+1e-3 );
			glTexCoord2f(0.0, 1.0);
			glVertex3f(xmin,ymax,zmin+1e-3);
			glTexCoord2f(1.0,1.0);
			glVertex3f(xmax,ymax,zmin+1e-3);
			glTexCoord2f(1.0,0.0);
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

	if( this->theForce->WallsAreOn() )
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
