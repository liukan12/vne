#include "TriObject.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "IOUtils.h"
#include <valarray>
#include <numeric>

//TriObject::TriObject(const Object& obj) : DrawObject( obj )
//{
//
//}


TriObject::TriObject(const valarray<double> NX, const valarray<double> NY, const valarray<double> NZ,
				const valarray<double> VX,const valarray<double> VY,const valarray<double> VZ,
				const valarray<int> IDX) : DrawObject( NX, NY, NZ, VX, VY, VZ, IDX )
{

	this->numFaces = IDX.size() / 3;
	this->numVerts = VX.size();
}
	
TriObject::~TriObject( )
{

}

void TriObject::setTexture(string fileName)
{
	this->objTexture=new VNETexture(fileName);
	this->bHasTexture=true;
}

void TriObject::setTexture(VNETexture* newTex)
{
}
void TriObject::DrawSelf()
{
	int i;
	
	glPushMatrix();
	
	if(this->bHasTexture)
	{	this->objTexture->bindTexture();
		for( i = 0; i < numFaces; i++ )
		{
			double offset = this->colorVariance*( i / double(numFaces) - 0.5 );
			glColor3f( rseed+offset, gseed+offset,bseed+offset);
			glBegin(GL_TRIANGLES);
				glNormal3d(CurNormX[ CurIdx[3*i+0] ], CurNormY[ CurIdx[3*i+0] ], CurNormZ[ CurIdx[3*i+0] ]);
				glTexCoord2f(0.0, 0.0);
				glVertex3d(CurVertX[ CurIdx[3*i+0] ], CurVertY[ CurIdx[3*i+0] ], CurVertZ[ CurIdx[3*i+0] ]);
				
				glNormal3d(CurNormX[ CurIdx[3*i+1] ], CurNormY[ CurIdx[3*i+1] ], CurNormZ[ CurIdx[3*i+1] ]);
				glTexCoord2f(1.0, 1.0);
				glVertex3d(CurVertX[ CurIdx[3*i+1] ], CurVertY[ CurIdx[3*i+1] ], CurVertZ[ CurIdx[3*i+1] ]);
				
				glNormal3d(CurNormX[ CurIdx[3*i+2] ], CurNormY[ CurIdx[3*i+2] ], CurNormZ[ CurIdx[3*i+2] ]);
				glTexCoord2f(0.0, 1.0);
				glVertex3d(CurVertX[ CurIdx[3*i+2] ], CurVertY[ CurIdx[3*i+2] ], CurVertZ[ CurIdx[3*i+2] ]);
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);

	}
	else
	{
		for( i = 0; i < numFaces; i++ )
		{

			double offset = this->colorVariance*( i / double(numFaces) - 0.5 );
			glColor3f( rseed+offset, gseed+offset,bseed+offset);
			glBegin(GL_TRIANGLES);
				glNormal3d(CurNormX[ CurIdx[3*i+0] ], CurNormY[ CurIdx[3*i+0] ], CurNormZ[ CurIdx[3*i+0] ]);
				glVertex3d(CurVertX[ CurIdx[3*i+0] ], CurVertY[ CurIdx[3*i+0] ], CurVertZ[ CurIdx[3*i+0] ]);
				glNormal3d(CurNormX[ CurIdx[3*i+1] ], CurNormY[ CurIdx[3*i+1] ], CurNormZ[ CurIdx[3*i+1] ]);
				glVertex3d(CurVertX[ CurIdx[3*i+1] ], CurVertY[ CurIdx[3*i+1] ], CurVertZ[ CurIdx[3*i+1] ]);
				glNormal3d(CurNormX[ CurIdx[3*i+2] ], CurNormY[ CurIdx[3*i+2] ], CurNormZ[ CurIdx[3*i+2] ]);
				glVertex3d(CurVertX[ CurIdx[3*i+2] ], CurVertY[ CurIdx[3*i+2] ], CurVertZ[ CurIdx[3*i+2] ]);
			glEnd();
		}
	}
	glPopMatrix();
	glFinish();

}
