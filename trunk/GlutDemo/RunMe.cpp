#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

// need to build a debug version of the GLUT dll to debug!!

static GLfloat spin = 0.0;

void init(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(spin/5.0,spin/5.0,-20.0);
	glRotatef(spin, 0.5, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_POLYGON);
		glVertex3f(0.0,25.0,0.0);
		glVertex3f(-25.0,0.0,0.0);
		glVertex3f(25.0,0.0,0.0);
		glVertex3f(0.0,-25.0,25.0);
		glVertex3f(-25.0,0.0,25.0);
		glVertex3f(25.0,0.0,25.0);
	glEnd();
	

	glLoadIdentity();
	glRotatef(spin, 0.5, 0.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);

	glBegin(GL_POLYGON);
		glVertex3f(0.0,25.0,0.0);
		glVertex3f(-25.0,0.0,0.0);
		glVertex3f(25.0,0.0,0.0);
		glVertex3f(0.0,-25.0,25.0);
		glVertex3f(-25.0,0.0,25.0);
		glVertex3f(25.0,0.0,25.0);
	glEnd();

	glutSwapBuffers();
}


void spinDisplay(void)
{
	spin = spin + 2.0;
	if (spin > 360.0)
		spin = spin - 360.0;
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50.0, -50.0, 50.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if( state == GLUT_DOWN)
				glutIdleFunc(spinDisplay);
			break;
		case GLUT_RIGHT_BUTTON:
			if( state == GLUT_DOWN )
				glutIdleFunc(NULL);
			break;
		default:
			break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMainLoop();

	int wtf = 1;
	wtf = wtf + 1;

	return 0;
}