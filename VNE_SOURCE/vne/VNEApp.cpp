#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "VNEApp.h"
#include "VNEWorld.h"
#include "VNEObject.h"
#include "CameraControl.h"

using namespace std;

#ifdef _DEBUG
int frame=0,mytime,timebase=0;
#endif

const double vxp[] = {5.0,0.0,0.0};
const double vxn[] = {-5.0,0.0,0.0};
const double vyp[] = {0.0,5.0,0.0};
const double vyn[] = {0.0,-5.0,0.0};
const double vzp[] = {0.0,0.0,5.0};
const double vzn[] = {0.0,0.0,-5.0};

const double xf[] = {20.0,0.0,0.0};
const double xs[] = {-20.0,0.0,0.0};
const double xyf[] = {20.0,-20.0,0.0};
const double xys[] = {-20.0,20.0,0.0};
const double yf[] = {0.0,200.0,0.0};
const double ys[] = {0.0,-200.0,0.0};
const double zf[] = {0.0,0.0,20.0};
const double zs[] = {0.0,0.0,-20.0};
valarray<double> XFaster(xf,3);
valarray<double> XSlower(xs,3);

valarray<double> XYFaster(xyf,3);
valarray<double> XYSlower(xys,3);

valarray<double> YFaster(yf,3);
valarray<double> YSlower(ys,3);
valarray<double> ZFaster(zf,3);
valarray<double> ZSlower(zs,3);

valarray<double> VXP(vxp,3);
valarray<double> VXN(vxn,3);
valarray<double> VYP(vyp,3);
valarray<double> VYN(vyn,3);
valarray<double> VZP(vzp,3);
valarray<double> VZN(vzn,3);

void processHits(GLint hits, GLuint buffer[] )
{
	unsigned int i,j;
	GLuint names, *ptr;

	printf("hits = %d\n", hits );
	ptr = (GLuint* ) buffer;
	for (i = 0; i < hits; i++ )
	{
		names = *ptr;
		printf(" number of names for hit = %d\n", names);
		ptr++;
		printf(" z1 is %g;", (float) *ptr/0x7fffffff);
		ptr++;
		printf(" z2 is %g\n", (float) *ptr/0x7fffffff);
		ptr++;
		printf("	CLICK EVENT AT OBJECT NUMBER: ");
		for( j = 0; j < names; j++ )
		{
			printf("%d ", *ptr);
			ptr++;
		}
		printf("\n");
	}
}

VNEApp::VNEApp()
{
	world = new VNEWorld();
	camera = new CameraControl( 0.0, 0.0, world->Getzmax(), world);
		
	iControlObjIdx = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void VNEApp::IdleCallback()
{
	DisplayCallback();
}

// x & y are current mouse cursor coordinates
// x = 1, y = 1 is the upper left cornder of the window
void VNEApp::KeyboardCallback(unsigned char key, int x, int y)
{	
	DemoObj = this->world->ObjList->GetObjectAt(iControlObjIdx);
	double currSpeed = DemoObj->GetSpeed( );
	double currSpin = DemoObj->GetAngSpeed( );
	if( key == 'r' )
		DemoObj->SetRotSpeed( -1*currSpin );
	if( key == 'w' )
		DemoObj->SetSpeed( currSpeed * 1.1 );
	if( key == 's' )
		DemoObj->SetSpeed( currSpeed * 0.9 );
	if( key == 'e' )
		DemoObj->SetRotSpeed( currSpin * 1.05 );
	if( key == 'd' )
		DemoObj->SetRotSpeed( currSpin * 0.95 );
	if( key == 'x' )
		DemoObj->ApplyInstTorque( XFaster );
	if( key == 'X' )
		DemoObj->ApplyInstTorque( XSlower );
	if( key == 'y' )
		DemoObj->ApplyInstTorque( YFaster );
	if( key == 'Y' )
		DemoObj->ApplyInstTorque( YSlower );
	if( key == 'z' )
		DemoObj->ApplyInstTorque( ZFaster );
	if( key == 'Z' )
		DemoObj->ApplyInstTorque( ZSlower );
	if( key == 'a' )
		DemoObj->ApplyInstTorque( XYFaster );
	if( key == 'A' )
		DemoObj->ApplyInstTorque( XYSlower );
	{ // control: forces
		if( key == ']' )
		{	world->EnableForce(0);
			cout<<"X-Y VORTEX ENABLED!!! \n"; }
		if( key == '[' )
		{	world->DisableForce(0);
			cout<<"X-Y VORTEX DISABLED!!! \n"; }
	}
	{  // control: acceleration
		if( key == 'i' )
			DemoObj->AddForceAllVerts( YFaster / double( DemoObj->GetNumVerts() ));
		if( key == 'k' )
			DemoObj->AddForceAllVerts( YSlower / double( DemoObj->GetNumVerts() ));
		if( key == 'j' )
			DemoObj->AddForceAllVerts( XSlower / double( DemoObj->GetNumVerts() ));
		if( key == 'l' )
			DemoObj->AddForceAllVerts( XFaster / double( DemoObj->GetNumVerts() ));
		if( key == 'o' )
			DemoObj->AddForceAllVerts( ZSlower / double( DemoObj->GetNumVerts() ));
		if( key == 'p' )
			DemoObj->AddForceAllVerts( ZFaster / double( DemoObj->GetNumVerts() ));
	}
	{   // control: translation
		if( key == 't' )
			DemoObj->SetVelocity( VYP );
		if( key == 'g' )
			DemoObj->SetVelocity( VYN );
		if( key == 'f' )
			DemoObj->SetVelocity( VXN );
		if( key == 'h' )
			DemoObj->SetVelocity( VXP );
	}
	{	// control:  camera
		if( key == 'v' )
			this->camera->TranslateBy(0.0,0.0,0.05);
		if( key == 'c' )
			this->camera->TranslateBy(0.0,0.0,-0.05);
		if( key == 'n' )
			this->camera->TranslateBy(0.05,0.0,0.0);
		if( key == 'b' )
			this->camera->TranslateBy(-0.05,0.0,0.0);
		if( key == '.' )
			this->camera->TranslateBy(0.0,0.05,0.0);
		if( key == ',' )
			this->camera->TranslateBy(0.0,-0.05,0.0);
		if( key == 'R' )
			this->camera->ResetPosition();
		if( key == '6' )
			this->camera->AttachToObject( this->DemoObj );
		if( key == '7' )
			this->camera->Detach();
	}
	{	// control:	lights
		if( key == '8' )
			this->world->LightsOn();
		if( key == '9' )
			this->world->LightsOff();
	}
	{	// control: object focus
		if( key == '1' )
			iControlObjIdx = 0;
		if( key == '2' )
			iControlObjIdx = 1;
		if( key == '3' )
			iControlObjIdx = 2;
		if( key == '4' )
			iControlObjIdx = 3;
	}

}

#define BUFSIZE 512
void VNEApp::MouseCallback( int button, int state, int x, int y)
{
	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];

	if( button != GLUT_LEFT_BUTTON || state != GLUT_DOWN )
		return;
	
	glGetIntegerv(GL_VIEWPORT, viewport );
	glSelectBuffer(BUFSIZE, selectBuf );
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix( (GLdouble) x, (GLdouble) (viewport[3]-y), 5.0, 5.0, viewport );

	int w,h;
	this->camera->GetWH( &w, &h );
	gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
	
	glMatrixMode(GL_MODELVIEW);
	this->world->ObjList->DoSelection();
	glPopMatrix();
	glFlush();

	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf );
	this->camera->ResizeCallbackHandler();
	
}

void VNEApp::DisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	this->world->TimeStep();
	this->world->Redraw();
	this->camera->UpdateAttachedCamera();
	
#ifdef _DEBUG
	frame++;
	mytime = glutGet(GLUT_ELAPSED_TIME);
	
	if (mytime - timebase > 3000) {
		cout<<"Frames per second: "<<frame*3000.0/(mytime-timebase)/2<<"\n";
	 	timebase = mytime;		
		frame = 0;
	}
#endif
	glColor3f(1.0,1.0,1.0); // clear color for background drawing
	glutSwapBuffers();

}

void VNEApp::ResizeCallback(int w, int h)
{
	this->camera->ResizeCallbackHandler( w, h );
}


