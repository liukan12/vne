#ifndef GL2D
#define GL2D
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Sprite.h"
class Gl2d
{
private:
	Sprite* testSprite;
	bool left,right,up,down;
public:
	Gl2d();
	~Gl2d();

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

	//keyboard
	void keyboard(unsigned char key, int x, int y);
	void keyboardup(unsigned char key, int x, int y);
	void special(int key, int x, int y);
};
#endif 
