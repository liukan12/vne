#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "VNEMortarApp.h"

using namespace std;

#ifdef _DEBUG
int frame=0,mytime,timebase=0;
#endif

VNEMortarApp::VNEMortarApp()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	Init();
}

void VNEMortarApp::IdleCallback()
{
	DisplayCallback();
}
void VNEMortarApp::Init()
{
	xmin = -110.0;
	xmax = 110.0;
	ymin = -110.0;
	ymax = 110.0;
	zmin = -60.0;
	zmax = 20.0;
	this->objControl = new ObjectController();
	this->camera = new CameraControl(0.0,2.0,15.0,zmin,zmax);
	this->mortar1 = new Mortar();
	this->mortar2 = new Mortar();
	iControlObjIdx = 0;

	//4 strings: faces, norms, verts, draw type
	vector<string> input1(4);
	vector<string> input2(4);
	
	input1[0] = "..\\vne_data\\facesmortar.dat"; 
	input1[1] = "..\\vne_data\\normsmortar.dat";
	input1[2] = "..\\vne_data\\vertsmortar.dat";
	input1[3] = "tri";
	input2[0] = "..\\vne_data\\facesmortar.dat"; 
	input2[1] = "..\\vne_data\\normsmortar.dat";
	input2[2] = "..\\vne_data\\vertsmortar.dat";
	input2[3] = "tri";

	mortar1->InitFromFile( input1 );
	mortar2->InitFromFile( input2 );
	mortar1->SetName("Mortar One");
	mortar2->SetName("Mortar Two");
	mortar1->TranslateTo(2.0,0.0,-35.0);
	mortar2->TranslateTo(-2.0,0.0,10.0);

	valarray<double> red1 = valarray<double>(mortar1->GetNumVerts() );
	red1 = 0.5;
	valarray<double> green1 = valarray<double>(mortar1->GetNumVerts() );
	green1 = 0.0;
	valarray<double> blue1 = valarray<double>(mortar1->GetNumVerts() );
	blue1 = 1.0;
	mortar1->GetDrawObj()->SetColors( red1, green1, blue1 );

	this->objControl->AddPhysObj(mortar1);
	this->objControl->AddPhysObj(mortar2);

	floorTex=new VNETexture("..\\vne_data\\ground.jpg");

	glClearColor(0.0, 0.0 , 0.0, 0.0 );
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

void VNEMortarApp::DrawWalls()
{
		
		floorTex->bindTexture();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glPushMatrix();
		glRotatef(-90.0,1.0,0.0,0.0);
		glBegin(GL_QUADS);
			// back wall
			glTexCoord2f(0.0, 0.0);
			glVertex3f(xmin, ymin, 0 );
			glTexCoord2f(0.0, 3.0);
			glVertex3f(xmin,ymax,0);
			glTexCoord2f(3.0,3.0);
			glVertex3f(xmax,ymax,0);
			glTexCoord2f(3.0,0.0);
			glVertex3f(xmax, ymin, 0 );
		glEnd();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);


}

valarray<double> tilt(3);

// x & y are current mouse cursor coordinates
// x = 1, y = 1 is the upper left cornder of the window
void VNEMortarApp::KeyboardCallback(unsigned char key, int x, int y)
{	
	tilt = 0.0;
	if( key == 'f' ) {
		Mortar* focusMortar = dynamic_cast<Mortar*>(this->objControl->GetPhysObj(this->iControlObjIdx));
		focusMortar->Fire();
	}

	//Tilt Controls
	if( key == 'a' ) {
		cout<<"x dir? ";
		cin>>tilt[0];
		cout<<"y dir? ";
		cin>>tilt[1];
		cout<<"z dir? ";
		cin>>tilt[2];
		Mortar* focusMortar = dynamic_cast<Mortar*>(this->objControl->GetPhysObj(this->iControlObjIdx));
		focusMortar->TiltTo(tilt);
	}
	
	// End Tilt Controls


	if( key == 'g' ) {
		this->objControl->GetPhysObj(this->iControlObjIdx)->GrabCamera(this->camera);
	}
	if( key == 'r' ) {
		this->camera->ResetPosition();
	}
	if( key == '1' ) {
		iControlObjIdx = 0;
	}
	if( key == '2' ) {
		iControlObjIdx = 1;
	}
	if( key == 'v' ) {
		this->objControl->GetPhysObj(this->iControlObjIdx)->SetVelocity(1.0,1.0,1.0);
	}
	if( key == 'i' ) {
		this->objControl->GetPhysObj(this->iControlObjIdx)->AddForceAllVerts(0.0,0.0,1.0);
		this->objControl->GetPhysObj(this->iControlObjIdx)->UpdateSelf();
	}
	if( key == 'k' ) {
		this->objControl->GetPhysObj(this->iControlObjIdx)->AddForceAllVerts(0.0,0.0,-1.0);
	}
	if( key == 'j' ) {
		this->objControl->GetPhysObj(this->iControlObjIdx)->AddForceAllVerts(-1.0,0.0,0.0);
	}
	if( key == 'l' ) {
		this->objControl->GetPhysObj(this->iControlObjIdx)->AddForceAllVerts(1.0,0.0,0.0);
	}

	
}

#define BUFSIZE 512;
void VNEMortarApp::MouseCallback(int button, int state, int x, int y)
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

void VNEMortarApp::DisplayCallback()
{
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
 
	//this->objControl->ApplyGravity(9.8 * 0.001 );
	this->objControl->Update();
	this->DrawWalls();
	glFinish();
	

	glutSwapBuffers();

}

void VNEMortarApp::ResizeCallback(int w, int h)
{
	this->camera->ResizeCallbackHandler( w, h );
}


