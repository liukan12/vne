#include "main.h"
#include "Gl2d.h"


//drawing functions go in this function
	void display()
	{
		gameApp->display();
	}

	//game logic goes here
	void update()
	{
		gameApp->update();
		glutPostRedisplay();
	}

	//window stuff 
	void reshape(int w, int h)
	{
		gameApp->reshape(w,h);
	}

	//mouse functions
	void motionPassive(int x, int y)
	{
		gameApp->motionPassive(x,y);
	}
	void motion(int x, int y)
	{
		gameApp->motion(x,y);
	}
	void mouse(int button, int state, int x, int y)
	{
		gameApp->mouse(button,state,x,y);
	}
	void init()
	{
	}
	void keyboard(unsigned char key, int x, int y)
	{
		gameApp->keyboard(key,x,y);
	}
	void keyboardup(unsigned char key, int x, int y)
{
	gameApp->keyboardup(key,x,y);
}
	void special(int key, int x, int y)
	{
		gameApp->special(key,x,y);
	}
int main(int argc, char** argv)
{
   glutInit(&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

   glutInitWindowSize (250, 250);

   glutInitWindowPosition (100, 100);

   glutCreateWindow (argv[0]);

   gameApp = new Gl2d();
   

  

   glutReshapeFunc(reshape);

   glutDisplayFunc(display); 

   glutIdleFunc(update);

   glutMotionFunc(motion);
   glutPassiveMotionFunc(motionPassive);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardup);
	glutSpecialFunc(special);
   glutMainLoop();

   return 0;
}