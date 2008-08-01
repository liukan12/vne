#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "VNEStereoApp.h"

using namespace std;

#ifdef _DEBUG
int frame=0,mytime,timebase=0;
#endif

int gameNum = 0;

VNEStereoApp::VNEStereoApp()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	Init();
}

void VNEStereoApp::IdleCallback()
{
	DisplayCallback();
}
void VNEStereoApp::Init()
{
	xmin = -10.0;
	xmax = 10.0;
	ymin = -10.0;
	ymax = 10.0;
	zmin = -10.0;
	zmax = 10.0;
	this->objControl = new ObjectController();
	this->camera = new CameraControl(0.0,0.0,5.0,zmin,zmax);
	iControlObjIdx = 0;

	//4 strings: faces, norms, verts, draw type
	vector<string> input1(4);
	input1[0] = "..\\vne_data\\facesmortar.dat"; 
	input1[1] = "..\\vne_data\\normsmortar.dat";
	input1[2] = "..\\vne_data\\vertsmortar.dat";
	input1[3] = "tri";

	obj1 = new PhysObject( input1 );
	objControl->AddPhysObj( obj1 );
	obj1->Scale( 0.05, 0.05, 0.05 );

	xcpos = vector<float>(9);
	ycpos = vector<float>(9);
	zcpos = vector<float>(9);
	xtpos = vector<float>(9);
	ytpos = vector<float>(9);
	ztpos = vector<float>(9);
	int k = 0;
	for( int i = -1; i <= 1; i+=1 ) {
		for( int j = -1; j <= 1; j+=1 ) {
			xcpos[k] = i;
			ycpos[k] = j;
			zcpos[k] = 0;
			xtpos[k] = i;
			ytpos[k] = j;
			ztpos[k] = 0;
			k++;
		}
	}


	obj1->TranslateTo( 0, 0, 0);
	camidx = 0;

	double objxmin, objxmax, objymin, objymax, objzmin, objzmax;
	obj1->GetMinMaxVert( &objxmin, &objxmax, &objymin, &objymax, &objzmin, &objzmax );

	valarray<double> red1 = valarray<double>(obj1->GetNumVerts() );
	valarray<double> green1 = valarray<double>(obj1->GetNumVerts() );
	valarray<double> blue1 = valarray<double>(obj1->GetNumVerts() );
	for( int i = 0; i < obj1->GetNumVerts(); i++ ) {
		red1[i] = i * 1.0/ obj1->GetNumVerts();
		green1[i] = 1 - i * 1.0/ obj1->GetNumVerts();;
		blue1[i] = i * 1.0 / obj1->GetNumVerts();
	}
	obj1->GetDrawObj()->SetColors( red1, green1, blue1 );
	
	cout<<"Starting run # "<<gameNum<<"\n";
	gameNum++;
	glClearColor(0.0, 0.0 , 0.0, 0.0 );
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

VNEStereoApp::~VNEStereoApp()
{
	delete this->objControl;
}

void VNEStereoApp::Reset()
{
	delete this->objControl;
	delete this->camera;
	Init();
}


// x & y are current mouse cursor coordinates
// x = 1, y = 1 is the upper left cornder of the window
void VNEStereoApp::KeyboardCallback(unsigned char key, int x, int y)
{	

	if( key == 'r' ) {
		this->camera->ResetPosition();
	}
	if( key == '1' ) {
		iControlObjIdx = 0;
	}
	if( key == '2' ) {
		iControlObjIdx = 1;
	}
	if( key == 'R' ) {
		cout<<"Resetting VNEStereo... \n";
		this->Reset();
	}


	
}

#define BUFSIZE 512;
void VNEStereoApp::MouseCallback(int button, int state, int x, int y)
{ /* NEED TO CREATE CAMERA TO USE THIS
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
	this->camera->ResizeCallbackHandler(); */
	
}

void VNEStereoApp::DisplayCallback()
{	
	// move the camera around and generate some data...
	double dx = this->xcpos[camidx];
	double dy = this->ycpos[camidx];
	cout<<"(NEXT) xpos: "<<dx<<"\t ypos: "<<dy<<"\n";
	this->camera->TranslateTo( dx, dy, 5.0 );
	this->camera->PointAt( dx, dy, 0.0 );
	this->camera->UpdateAttachedCamera();
	camidx++;
	camidx = (camidx < 9 ) ? camidx : 0;

	this->objControl->Update();
	glFinish();
	glutSwapBuffers();

	
	
	
#ifdef _DEBUG
	frame++;
	mytime = glutGet(GLUT_ELAPSED_TIME);
	
	if (mytime - timebase > 3000) {
		cout<<"Frames per second: "<<frame*3000.0/(mytime-timebase)/2<<"\n";
	 	timebase = mytime;		
		frame = 0;
	}
#endif

}

void VNEStereoApp::ResizeCallback(int w, int h)
{
	this->camera->ResizeCallbackHandler( w, h );
}


