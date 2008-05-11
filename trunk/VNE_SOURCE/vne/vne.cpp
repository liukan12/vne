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
#include "VNEApp.h"

using std::cout;
VNEApp* app;

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
	app->MouseCallback();
}

void keyboard(unsigned char key, int x, int y)
{
	app->KeyboardCallback();
}

int main( int argc, char* argv[] )
{
	//glDrawElements(GL_TRIANGLES,3*4, GL_UNSIGNED_BYTE, allidx );
	cout<<"Welcome to VNE 0.0! \n";
	
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);

	app = new VNEApp();

	glutDisplayFunc( display );
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;


}