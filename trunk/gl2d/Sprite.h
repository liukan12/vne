#ifndef SPRITE_H
#define SPRITE_H
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <SOIL.h>
#include <iostream>
#include <string>

class Sprite
{
private:
	
	
	GLuint image;
	float width, height;
public:
	float posX,posY;
	float rotation;
	Sprite(std::string filename);
	~Sprite();
	
	void Draw();
	void Draw(float x, float y);
	void SetPosition(float x, float y);
	void SetRotation(float angle);
	void SetWidth(float w);
	void SetHeight(float h);
	float Width();
	float Height();
	
	void  GetDimensions(float *dimensionsV);
	void Scale(float scaleX, float scaleY);
	
};
#endif
