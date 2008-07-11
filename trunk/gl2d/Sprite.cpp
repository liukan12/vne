#include "Sprite.h"
using namespace std;
Sprite::Sprite(string filename)
{
	
	image = SOIL_load_OGL_texture
	(
		filename.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y|SOIL_FLAG_MULTIPLY_ALPHA
	);
	GLint w,h;
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&w);
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&h);
	width=w;
	height=h;
	cout<<"width:"<< width<<"height:"<<height<<endl;

	posX=0;
	posY=0;

	rotation=0;
}
Sprite::~Sprite()
{
}
void Sprite::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,image);
    glPushMatrix();
		glLoadIdentity();
		glTranslatef(posX,posY,0);
		
		
		glRotatef(rotation,0.0,0.0,1.0);
		glTranslatef(-(width/2.0),-(height/2.0),0.0);
		
		
		
		glBegin(GL_QUADS);
			glTexCoord2f(0.0,0.0); glVertex2f(0.0,0.0);
			glTexCoord2f(1.0, 0.0);glVertex2f(width, 0.0);
			glTexCoord2f(1.0,1.0);glVertex2f(width,height);
			glTexCoord2f(0.0,1.0);glVertex2f(0.0,height);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void Sprite::Draw(float x, float y)
{
	posX=x;
	posY=y;
	this->Draw();
}
void Sprite::SetPosition(float x, float y)
{
	posX=x;
	posY=y;
}
void Sprite::SetRotation(float angle)
{
	rotation=angle;
}
void Sprite::SetWidth(float w)
{
	width=w;
}
void Sprite::SetHeight(float h)
{
	height=h;
}
float Sprite::Width()
{
	return width;
}
float Sprite::Height()
{
	return height;
}

void  Sprite::GetDimensions(float *dimensionsV)
{
	dimensionsV[0]=posX;
	dimensionsV[1]=posY;
}
void Sprite::Scale(float scaleX, float scaleY)
{
	width*=scaleX;
	height*=scaleY;
}