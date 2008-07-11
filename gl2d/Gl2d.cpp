#include "Gl2d.h"

Gl2d::Gl2d()
{
	
	testSprite=new Sprite("test.png");
	glClearColor (0.0, 0.0, 0.0, 0.0);
	testSprite->SetPosition(200,200);
	right=false;
	left=false;
	up=false;
	down=false;
}

Gl2d::~Gl2d()
{
}

//drawing functions go in this function
void Gl2d::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	testSprite->Draw();
	glFlush();
	glutSwapBuffers();
	
}

//game logic goes here
void Gl2d::update()
{
	testSprite->rotation+=2;
	if(testSprite->rotation>360)
		testSprite->rotation=0;
	if(left)
		testSprite->posX-=2;
	if(right)
		testSprite->posX+=2;
	if(up)
		testSprite->posY+=2;
	if(down)
		testSprite->posY-=2;

}

//window stuff 
void Gl2d::reshape(int w, int h)
{
	glViewport(0, 0, w, h);		// reset the viewport to new dimensions
	glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
	glLoadIdentity();						// reset projection matrix

	// calculate aspect ratio of window
	glOrtho(0.0f, (float)w - 1.0, 0.0, (float)h - 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);				// set modelview matrix
	glLoadIdentity();
}

//mouse functions
void Gl2d::motionPassive(int x, int y)
{
}
void Gl2d::motion(int x, int y)
{
}
void Gl2d::mouse(int button, int state, int x, int y)
{
}
void Gl2d::keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	// Backspace
	case 'a' :
		left=true;
		break;

	// Enter
	case 'd' :
		right=true;
		break;

	// Escape
	case 's':
		down=true;
		break;

	// Delete
	case 'w' :
		up=true;
		break;
	default:
		break;
	}
}
void Gl2d::keyboardup(unsigned char key, int x, int y)
{
	switch(key)
	{
	// Backspace
	case 'a' :
		left=false;
		break;

	// Enter
	case 'd' :
		right=false;
		break;

	// Escape
	case 's':
		down=false;
		break;

	// Delete
	case 'w' :
		up=false;
		break;
	default:
		break;
	}
}
void Gl2d::special(int key, int x, int y)
	{
	}