/* The file containing the main() function
** Launches the program, initializes things,
** Sets into the main loop of display , check events, display, ..
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "VNEMortarApp.h"

using namespace std;

#define ETHAN_GAME
#ifndef ETHAN_GAME
VNEApp* app;
#else
VNEMortarApp* app;
#endif

// NOTE: THIS SHOULD BE THE ONLY FILE WHERE GLUT IS CALLED !!!

void display(void)
{
	app->DisplayCallback();
}

void reshape(int w, int h)
{
	app->ResizeCallback(w,h);
}

void idle(void)
{
	app->IdleCallback();
}

void mouse(int button, int state, int x, int y)
{
	app->MouseCallback(button, state, x, y);
}

void keyboard(unsigned char key, int x, int y)
{
	app->KeyboardCallback(key, x, y);


}

int main( int argc, char* argv[] )
{
	cout<<"Welcome to VNE Mortar! \n";

	glutInit( &argc, argv );
	//cout<<"done glutInit...\n";
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);

	cout<<"\n Your System Properties: \n";
	cout<<"OpenGL version: "<<glGetString( GL_VERSION )<<"\n";
	cout<<"Vendor: "<<glGetString( GL_VENDOR )<<"\n";
	//cout<<"Extensions: "<<glGetString( GL_EXTENSIONS )<<"\n";
	cout<<"Renderer: "<<glGetString( GL_RENDERER )<<"\n";

	//cout<<"creating app...\n";
#ifdef ETHAN_GAME
	app = new VNEMortarApp();
#else
	app = new VNEApp();
#endif
	////cout<<"app is made...\n";
	glutDisplayFunc( display );
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	//glutFullScreen();
	glutMainLoop();

	return 0;


}