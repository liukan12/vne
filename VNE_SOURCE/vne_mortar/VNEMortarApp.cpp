#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "VNEMortarApp.h"
#include "Globals.h"

using namespace std;

#ifdef _DEBUG
int frame=0,mytime,timebase=0;
#endif

int gameNum = 0;

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
	vector<string> input3(4);
	vector<string> input4(4);
	vector<string> input5(4);
	
	input1[0] = "..\\vne_data\\facesmortar.dat"; 
	input1[1] = "..\\vne_data\\normsmortar.dat";
	input1[2] = "..\\vne_data\\vertsmortar.dat";
	input1[3] = "tri";
	input2[0] = "..\\vne_data\\facesmortar.dat"; 
	input2[1] = "..\\vne_data\\normsmortar.dat";
	input2[2] = "..\\vne_data\\vertsmortar.dat";
	input2[3] = "tri";
	input3[0] = "..\\vne_data\\facesmortar.dat"; 
	input3[1] = "..\\vne_data\\normsmortar.dat";
	input3[2] = "..\\vne_data\\vertsmortar.dat";
	input3[3] = "tri";
	input4[0] = "..\\vne_data\\facesmortar.dat"; 
	input4[1] = "..\\vne_data\\normsmortar.dat";
	input4[2] = "..\\vne_data\\vertsmortar.dat";
	input4[3] = "tri";
	input5[0] = "..\\vne_data\\facesmortar.dat"; 
	input5[1] = "..\\vne_data\\normsmortar.dat";
	input5[2] = "..\\vne_data\\vertsmortar.dat";
	input5[3] = "tri";

	Obstacle* obst = new Obstacle();
	obst->InitFromFile( input3 );
	obst->TranslateTo( 8.0,10.0,-10.0 );
	obst->SetName("Obstacle One");
	Obstacle* obst2 = new Obstacle();
	obst2->InitFromFile( input4 );
	obst2->TranslateTo( -8.0,5.0,0.0 );
	obst2->SetName("Obstacle Two");
	Obstacle* obst3 = new Obstacle();
	obst3->InitFromFile( input5 );
	obst3->TranslateTo( 0.0,5.0,-15.0 );
	obst3->SetName("Obstacle Three");

	mortar1->InitFromFile( input1 );
	mortar2->InitFromFile( input2 );
	mortar1->SetName("Mortar One");
	mortar2->SetName("Mortar Two");
	mortar1->TranslateTo(4.0,0.0,-35.0);
	mortar2->TranslateTo(-4.0,0.0,10.0);

	valarray<double> initTilt1(3);
	initTilt1[0] = 0;
	initTilt1[1] = 1;
	initTilt1[2] = 2;
	mortar1->TiltTo( initTilt1 );
	initTilt1[0] = 0;
	initTilt1[1] = 1;
	initTilt1[2] = -2;
	mortar2->TiltTo( initTilt1 );

	valarray<double> red1 = valarray<double>(mortar1->GetNumVerts() );
	valarray<double> green1 = valarray<double>(mortar1->GetNumVerts() );
	valarray<double> blue1 = valarray<double>(mortar1->GetNumVerts() );
	valarray<double> red2 = valarray<double>(obst->GetNumVerts() );
	valarray<double> green2 = valarray<double>(obst->GetNumVerts() );
	valarray<double> blue2 = valarray<double>(obst->GetNumVerts() );
	valarray<double> red3 = valarray<double>(obst2->GetNumVerts() );
	valarray<double> green3 = valarray<double>(obst2->GetNumVerts() );
	valarray<double> blue3 = valarray<double>(obst2->GetNumVerts() );
	valarray<double> red4 = valarray<double>(obst3->GetNumVerts() );
	valarray<double> green4 = valarray<double>(obst3->GetNumVerts() );
	valarray<double> blue4 = valarray<double>(obst3->GetNumVerts() );
	for( int i = 0; i < mortar1->GetNumVerts(); i++ ) {
		red1[i] = i * 1.0/ mortar1->GetNumVerts();
		green1[i] = 0.0;
		blue1[i] = i * 1.0 / mortar1->GetNumVerts();
		red2[i] = 0.0;
		green2[i] = 0.5 + (rand()%7)/20.0;
		blue2[i] = 0.5 + (rand()%7)/20.0;
		red3[i] = 0.5 + (rand()%7)/20.0;
		green3[i] = 0.5 + (rand()%7)/20.0;
		blue3[i] = 0.5 + (rand()%7)/20.0;
		red4[i] = 0.2 + (rand()%7)/20.0;
		green4[i] = 0.0;
		blue4[i] = 0.9 + (rand()%7)/20.0;

	}
	mortar1->GetDrawObj()->SetColors( red1, green1, blue1 );
	obst->GetDrawObj()->SetColors( red2, green2, blue2 );
	obst2->GetDrawObj()->SetColors( red3, green3, blue3 );
	obst3->GetDrawObj()->SetColors( red4, green4, blue4 );

	this->objControl->AddPhysObj(mortar1);
	this->objControl->AddPhysObj(mortar2);
	this->objControl->AddPhysObj(obst);
	this->objControl->AddPhysObj(obst2);
	this->objControl->AddPhysObj(obst3);

	mortar2->GrabCamera( camera, mortar1 );

	if( gameNum == 0 ) {
		floorTex=new VNETexture("..\\vne_data\\ground.jpg");
		this->backTex = NULL;
	}
	cout<<"Starting game # "<<gameNum<<"\n";
	gameNum++;
	glClearColor(0.0, 0.0 , 0.0, 0.0 );
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

VNEMortarApp::~VNEMortarApp()
{
	if( this->floorTex )
		delete floorTex;
	delete this->objControl;
}

void VNEMortarApp::Reset()
{
	delete this->objControl;
	delete this->camera;
	Init();
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

	Mortar* focusMortar = dynamic_cast<Mortar*>(this->objControl->GetPhysObj(this->iControlObjIdx));
	Mortar* targetMortar;

	if(iControlObjIdx == 0)
		targetMortar = dynamic_cast<Mortar*>(this->objControl->GetPhysObj(1));
	else if (iControlObjIdx == 1)
		targetMortar = dynamic_cast<Mortar*>(this->objControl->GetPhysObj(0));

	if( !targetMortar || !focusMortar )
		return;

	tilt = 0.0;
	if( key == 'c' ) {
		valarray<double> delta = ( targetMortar->GetCentroid() - focusMortar->GetCentroid() );
		double L = sqrt( (delta*delta).sum() );
		double minE = -GRAV * 0.5 * L; // mass of shell is 1.0
		cout<<"Challenge: hit target with minimum possible kinetic energy. \n ";
		cout<<"Computer predicts this to be mgL/2 = "<<minE<<"\n";
	}
	if( key == 'f' ) {
		focusMortar->Fire();
		
		if( iControlObjIdx == 0 ) {
			cout<<"Player 2 has next turn! \n";
			iControlObjIdx = 1;
		}
		else {
			iControlObjIdx = 0;
			cout<<"Player 1 has next turn! \n";
		}
	}

	//Tilt Controls
	if( key == 'a' ) {
		cout<<"x dir? ";
		cin>>tilt[0];
		cout<<"y dir? ";
		cin>>tilt[1];
		cout<<"z dir? ";
		cin>>tilt[2];
		
		focusMortar->TiltTo(tilt);
	}
	
	// End Tilt Controls


	if( key == 'g' ) {
		focusMortar->GrabCamera(this->camera, targetMortar);
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
	if( key == 'i' ) {
		this->objControl->GetPhysObj(this->iControlObjIdx)->TranslateBy(0.0,0.0,0.05);
	}
	if( key == 'k' ) {
		this->objControl->GetPhysObj(this->iControlObjIdx)->TranslateBy(0.0,0.0,-0.05);
	}
	if( key == 'j' ) {
		this->objControl->GetPhysObj(this->iControlObjIdx)->TranslateBy(-0.05,0.0,0.0);
	}
	if( key == 'l' ) {
		this->objControl->GetPhysObj(this->iControlObjIdx)->TranslateBy(0.05,0.0,0.0);
	}

	//power adjustments
	if( key == '=' ) {
		focusMortar->SetPower( focusMortar->GetPower()+0.5 );
		cout<<focusMortar->Name().c_str()<<" has power of "<<focusMortar->GetPower()<<"\n";
	}
	if( key == '+' ) {
		focusMortar->SetPower( focusMortar->GetPower() + 0.05 );
		cout<<focusMortar->Name().c_str()<<" has power of "<<focusMortar->GetPower()<<"\n";
	}
	if( key == '-' ) {
		focusMortar->SetPower( focusMortar->GetPower()-0.5 );
		cout<<focusMortar->Name().c_str()<<" has power of "<<focusMortar->GetPower()<<"\n";
	}
	if( key == '_' ) {
		focusMortar->SetPower( focusMortar->GetPower() - 0.05 );
		cout<<focusMortar->Name().c_str()<<" has power of "<<focusMortar->GetPower()<<"\n";
	}

	valarray<double> newdir(3);
	
	if( key == 'v' ) {
		newdir = focusMortar->GetDir();
		newdir[0] += 0.01;
		focusMortar->TiltTo( newdir );
	}
	if( key == 'V' ) {
		newdir = focusMortar->GetDir();
		newdir[0] -= 0.01;
		focusMortar->TiltTo( newdir );
	}
	if( key == 'b' ) {
		newdir = focusMortar->GetDir();
		newdir[1] += 0.01;
		focusMortar->TiltTo( newdir );
	}
	if( key == 'B' ) {
		newdir = focusMortar->GetDir();
		newdir[1] -= 0.01;
		focusMortar->TiltTo( newdir );
	}
	if( key == 'n' ) {
		newdir = focusMortar->GetDir();
		newdir[2] += 0.01;
		focusMortar->TiltTo( newdir );
	}
	if( key == 'N' ) {
		newdir = focusMortar->GetDir();
		newdir[2] -= 0.01;
		focusMortar->TiltTo( newdir );
	}

	if( key == 'R' ) {
		cout<<"Resetting VNEMortar... \n";
		this->Reset();
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
	this->objControl->Update();
	this->DrawWalls();
	glFinish();
	glutSwapBuffers();

	this->camera->UpdateAttachedCamera();
	Mortar* focusMortar = dynamic_cast<Mortar*>(this->objControl->GetPhysObj(this->iControlObjIdx));
	Mortar* targetMortar;

	if(iControlObjIdx == 0)
		targetMortar = dynamic_cast<Mortar*>(this->objControl->GetPhysObj(1));
	else if (iControlObjIdx == 1)
		targetMortar = dynamic_cast<Mortar*>(this->objControl->GetPhysObj(0));

	int numObjs = this->objControl->NumPhysObjs();

	if( numObjs < (3+3) ) {
		// if there are more than 2 objects, there is a shell flying
		// let it land before updating the view
		if( targetMortar && focusMortar )
			focusMortar->GrabCamera(camera, targetMortar );

		if( numObjs < (2+3) ) {
			char ans = 'b';
			while( ans != 'y' && ans != 'Y' && ans != 'n' && ans != 'N' ) {
				cout<<"A mortar has been destroyed; play again? (y/n) \n";
				cin>>ans;
			}
			if( ans == 'y' || ans == 'Y' ) {
				this->Reset();
			}
			else
				exit(0);
			
			return;
		}
	}

	
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

void VNEMortarApp::ResizeCallback(int w, int h)
{
	this->camera->ResizeCallbackHandler( w, h );
}


