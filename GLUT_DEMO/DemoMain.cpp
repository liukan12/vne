#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glut.h>

using std::cout;
using std::cin;

static int year = 0;
static int day = 0;
static int yy = 0;
static int xx = 0;

void init(void)
{
	glClearColor(0.0, 0.0 , 0.0, 0.0 );
	glShadeModel(GL_FLAT);
}

void display(void)
{
	day = day + 5;
	year = year + 1;

	if( year >= 360 )
		year = 0;
	if( day >= 360 )
		day = 0;
	
	int hm = rand();

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
	glRotatef( -(GLfloat) (day/36)*(day/36), 0.0, 1.0, 0.0 );
	glutWireCube(2.0);
	glPopMatrix();
	glPushMatrix();
	glRotatef( (GLfloat) year, 0.0, 1.0, 0.0 );
	glTranslatef(2.0+xx, 0.0, 0.0);
	glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
	glutWireSphere(0.2, 10, 8 ); // a planet
	glTranslatef(yy/2, xx/2, 0.0 );
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) 
	{
	case 'd':
		day = (day + 10)%360;
		glutPostRedisplay();
		break;
	case 'D':
		day = (day - 10) % 360;
		glutPostRedisplay();
		break;
	case 'y':
		year = (year + 5)%360;
		glutPostRedisplay();
		break;
	case 'Y':
		year = (year + 5)%360;
		glutPostRedisplay();
		break;
	default:
		cout<<"you have pressed the "<<key<<" key! \n ";
		break;
	}
}

int main( int argc, char* argv [] )
{
	cout<<"wtf 1.0 \n";

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}