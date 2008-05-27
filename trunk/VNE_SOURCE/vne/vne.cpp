/* The file containing the main() function
** Launches the program, initializes things,
** Sets into the main loop of display , check events, display, ..
*/
//#include <stdio.h>
//#include <stdlib.h>
#include <windows.h>
//#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "VNEApp.h"

//using std:://cout;
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
	cout<<"Welcome to VNE 0.35! \n";
	cout<<"Key Commands:\n";
	cout<<"w  :  increase speed\n";
	cout<<"s  :  decrease speed\n";
	cout<<"e  :  increase spin speed\n";
	cout<<"d  :  decrease spin speed\n";
	cout<<"x  :  tilt onto x-axis\n";
	cout<<"y  :  tilt onto y-axis\n";
	cout<<"z  :  tilt onto z-axis\n";
	cout<<"i  :  accelerate object up\n";
	cout<<"k  :  accelerate object down\n";
	cout<<"j  :  accelerate object left\n";
	cout<<"l  :  accelerate object right\n";
	cout<<"t  :  move object up\n";
	cout<<"g  :  move object down\n";
	cout<<"f  :  move object left\n";
	cout<<"h  :  move object right\n";
	cout<<"1  :  transfer control to Object 1\n";
	cout<<"2  :  transfer control to Object 1\n";
	cout<<"3  :  transfer control to Object 1\n";
	cout<<"]  :  Enable X-Y Black Hole\n";
	cout<<"[  :  Disable X-Y Black Hole\n";
	cout<<"6  :  Attach Camera To Current Object\n";
	cout<<"7  :  Detach Camera From Object\n";
	cout<<"8  :  Enable Light-Mode (disable Colors)\n";
	cout<<"9  :  Enable Color-Mode (disable Lights)\n";
	cout<<"Shift-r : Reset Camera View\n";

	glutInit( &argc, argv );
	//cout<<"done glutInit...\n";
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);

	//cout<<"creating app...\n";
	app = new VNEApp();
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