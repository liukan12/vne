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
	app->KeyboardCallback(key, x, y);
}

int main( int argc, char* argv[] )
{
	//glDrawElements(GL_TRIANGLES,3*4, GL_UNSIGNED_BYTE, allidx );
	cout<<"Welcome to VNE 0.1! \n";
	cout<<"Key Commands:\n";
	cout<<"w  :  increase rotation speed w/ constant period \n";
	cout<<"s  :  increase rotation speed w/ constant period\n";
	cout<<"e  :  increase spin speed\n";
	cout<<"d  :  decrease spin speed\n";
	cout<<"x  :  spin about x-axis\n";
	cout<<"y  :  spin about y-axis\n";
	cout<<"z  :  spin about z-axis\n";
	cout<<"i  :  move object up\n";
	cout<<"k  :  move object down\n";
	cout<<"j  :  move object left\n";
	cout<<"l  :  move object right\n";
	cout<<"Caution: there is *no fixed position*, all rotations are self-referenced; \n";
	cout<<"Using 'w' key too much can cause instability as a result (too large of time steps)\n\n";
	cout<<"moving with ijkl is tricky if object is rotating; hold down s so it stops, then move it around\n";
	cout<<"it is possible that holding down s or d long enough";
	cout<<"will make it impossible to speed back up (numerical underflow)\n";

 glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
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