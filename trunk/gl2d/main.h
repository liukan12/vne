#ifndef PMAIN_H
#define PMAIN_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Gl2d.h"



int main(int argc, char** argv);
//drawing functions go in this function
	void display();

	//game logic goes here
	void update();

	//window stuff 
	void reshape(int w, int h);

	//mouse functions
	void motionPassive(int x, int y);
	void motion(int x, int y);
	void mouse(int button, int state, int x, int y);

	void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void keyboardup(unsigned char key, int x, int y);
	void init();
	Gl2d* gameApp;

#endif //end of main