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
#include "SOIL.h"
#include "VNETexture.h"
#include <numeric>
#include <valarray>

using namespace std;

VNEWorld::VNEWorld()
{
	string faces, faces2, faces3, faces4;
	string verts, verts2, verts3, verts4;
	string norms, norms2, norms3, norms4;
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
	faces4 = "..\\vne_data\\brainfaces.dat";
	verts4 = "..\\vne_data\\brainverts.dat";
	norms4 = "..\\vne_data\\brainnorms.dat";
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
	VNEObject* Obj4 = new VNEObject( "object 4", faces4, verts4, norms4); // no norms for the brain
#ifdef _DEBUG
	Obj1->setTexture("..\\vne_data\\obj1.png");
	Obj2->setTexture("..\\vne_data\\obj2.png");
	Obj3->setTexture("..\\vne_data\\obj3.png");
#else
	Obj1->setTexture(".\\data\\obj1.png");
	Obj2->setTexture(".\\data\\obj2.png");
	Obj3->setTexture(".\\data\\obj3.png");
#endif
	
	Obj1->SetVelocityProfile( 0.5, 0.5, 0.0, 0 );
	Obj2->SetVelocityProfile( -0.5, 0.5, 0.5, 0 );
	Obj3->SetVelocityProfile( 0.5, -0.5, -0.5, 0 );
	Obj4->SetVelocityProfile( -0.5, -0.5, -0.5, 0 );
	
	Obj1->SetColorSeed(0.5,0.0,1.0);
	Obj2->SetColorSeed(1.0,0.0,0.5);
	Obj3->SetColorSeed(0.5,0.5,0.0);
	Obj4->SetColorSeed(0.5,0.5,0.5);

	Obj1->TranslateTo(2.0,2.0,2.0);
	Obj2->TranslateTo(-2.0,-2.0,-2.0);
	Obj3->TranslateTo(4.0,1.0,-2.0);
	Obj4->TranslateTo(1.0,-3.0,1.0);
	this->ObjList = new VNEObjList( Obj1 );
	this->ObjList->AddObj(Obj2);
	this->ObjList->AddObj(Obj3);
	this->ObjList->AddObj(Obj4);

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
	
#ifdef _DEBUG
	myTex=new VNETexture("..\\vne_data\\cat.jpg");
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
		break;
	default:
		cout<<"Invalid number in DisableForce()!\n";
		break;
	}
}
void VNEWorld::Collide(VNEObject* obj1, VNEObject* obj2)
{
	double vx1, vy1, vz1;
	double vx2, vy2, vz2;
	double cx1, cy1, cz1;
	double cx2, cy2, cz2;

	double m1 = obj1->GetMass();
	double m2 = obj2->GetMass();
	double totalM = m1+m2;



	vx1 = (*obj1->GetVelocity())[0];
	vy1 = (*obj1->GetVelocity())[1];
	vz1 = (*obj1->GetVelocity())[2];
	vx2 = (*obj2->GetVelocity())[0];
	vy2 = (*obj2->GetVelocity())[1];
	vz2 = (*obj2->GetVelocity())[2];
	
	obj1->GetCentroid(&cx1, &cy1, &cz1 );
	obj2->GetCentroid(&cx2, &cy2, &cz2 );


	// TODO: how does impact affect the spin of the object??



	double scale_n1 = vx1 * (cx2-cx1) + vy1 * (cy2-cy1) + vz1 * (cz2-cz1);
	double normFac = ( (cx2-cx1)*(cx2-cx1) + (cy2-cy1)*(cy2-cy1) + (cz2-cz1)*(cz2-cz1) );
	scale_n1 = scale_n1 / normFac;
	double un1x, un1y, un1z; // normal velocity before collision of object 1
	un1x = scale_n1 * (cx2-cx1);
	un1y = scale_n1 * (cy2-cy1);
	un1z = scale_n1 * (cz2-cz1);
	double scale_n2 = vx2 * (cx1-cx2) + vy2 * (cy1-cy2) + vz2 * (cz1-cz2);
	scale_n2 = scale_n2 / normFac;
	double un2x, un2y, un2z; // normal velocity before collision of object 1
	un2x = scale_n2 * (cx1-cx2);
	un2y = scale_n2 * (cy1-cy2);
	un2z = scale_n2 * (cz1-cz2);

	// tangential components of objs 1 and 2; these do not change with collision
	double ut1x, ut1y, ut1z;
	double ut2x, ut2y, ut2z;
	ut1x = vx1 - un1x;
	ut1y = vy1 - un1y;
	ut1z = vz1 - un1z;
	ut2x = vx2 - un2x;
	ut2y = vy2 - un2y;
	ut2z = vz2 - un2z;

#ifdef _DEBUG
	double normV1 = vx1*vx1 + vy1*vy1 + vz1*vz1;
	double normV1t = ut1x*ut1x + ut1y*ut1y + ut1z*ut1z;
	double normV1n = un1x*un1x + un1y*un1y + un1z*un1z;
	double dRoundErr = normV1 - normV1t - normV1n;
	if( dRoundErr > 1e-5 )
		cout<<"Error in accuracy of collision computation... error: "<<dRoundErr<<"\n";
#endif

	double vn1x, vn1y, vn1z;
	double vn2x, vn2y, vn2z;

	vn1x = ( un1x*(m1-m2) + 2*m2*un2x ) / totalM;
	vn1y = ( un1y*(m1-m2) + 2*m2*un2y ) / totalM;
	vn1z = ( un1z*(m1-m2) + 2*m2*un2z ) / totalM;
	vn2x = ( un2x*(m2-m1) + 2*m2*un1x ) / totalM;
	vn2y = ( un2y*(m2-m1) + 2*m2*un1y ) / totalM;
	vn2z = ( un2z*(m2-m1) + 2*m2*un1z ) / totalM;

	vx1 = vn1x + ut1x;
	vy1 = vn1y + ut1y;
	vz1 = vn1z + ut1z;
	vx2 = vn2x + ut2x;
	vy2 = vn2y + ut2y;
	vz2 = vn2z + ut2z;
	
	obj1->SetVelocityProfile(vx1,vy1,vz1,0);
	obj2->SetVelocityProfile(vx2,vy2,vz2,0);
	
	double dMinDist = obj1->GetRadSquared() + obj2->GetRadSquared();
	double dCurrDist = (cx1 - cx2)*(cx1 - cx2) + (cy1 - cy2)*(cy1 - cy2) + (cz1 - cz2)*(cz1 - cz2);

	int max_its = 100;
	int k = 0;
	while( dCurrDist < dMinDist && k < max_its )
	{
		obj1->TranslateBy( .001*vx1, .001*vy1, .001*vz1 );
		obj2->TranslateBy( .001*vx2, .001*vy2, .001*vz2 );
		obj1->GetCentroid(&cx1, &cy1, &cz1 );
		obj2->GetCentroid(&cx2, &cy2, &cz2 );
		dCurrDist = (cx1 - cx2)*(cx1 - cx2) + (cy1 - cy2)*(cy1 - cy2) + (cz1 - cz2)*(cz1 - cz2);
		k++;
	}

}

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
			double thresh = obj1->GetRadSquared() + obj2->GetRadSquared();
			valarray<double> diff = *obj1->GetCentroid() - *obj2->GetCentroid();
			diff = diff * diff;
			if(thresh > diff.sum() ) // TODO: difference of centroids
			{
				this->Collide( obj1, obj2);
			}
		}
	}
}

int VNEWorld::TimeStep()
{
	int result = 0;

	clock2 = clock();
	double dt = ( (double) (clock2 - clock1) ) / CLOCKS_PER_SEC ;
	elapsedTime += dt;
	clock1 = clock();

	this->ObjList->AccelAll( this->theForce );
	result = this->ObjList->TimeStepAll();
	this->CheckCollisions();
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
		
		myTex->bindTexture();
		

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
