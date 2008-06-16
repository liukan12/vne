#include "TriObject.h"

TriObject::TriObject(const valarray<double> &NX, const valarray<double> &NY, 
					 const valarray<double> &NZ, const valarray<double> &VX, 
					 const valarray<double> &VY, const valarray<double> &VZ, 
					 const valarray<int> &IDX) : DrawObject( NX, NY, NZ, VX, VY, VZ, IDX )
{
	this->numVerts = VX.size();
	this->numFaces = IDX.size()/3;
	RedVert = valarray<double>(numVerts);
	BluVert = valarray<double>(numVerts);
	GrnVert = valarray<double>(numVerts);
	for( int i = 0; i < numVerts; i++ ) {
		RedVert[i] = double(i)/numVerts;
		BluVert[i] = double(i)/numVerts;
		GrnVert[i] = double(i)/numVerts;
	}

}

TriObject::~TriObject()
{
	
}

void TriObject::DrawSelf()
{
	int i;
	
	glPushMatrix();
	if(this->bHasTexture)	{	
		this->objTexture->bindTexture();
		for( i = 0; i < numFaces; i++ ) 	{
			//glColor3f( rseed, gseed,bseed);
			glBegin(GL_TRIANGLES);
				glColor3f( RedVert[CurIdx[3*i+0]], GrnVert[	CurIdx[3*i+0] ], BluVert[ CurIdx[3*i+0] ] );
				//glNormal3d(CurNormX[ CurIdx[3*i+0] ], CurNormY[ CurIdx[3*i+0] ], CurNormZ[ CurIdx[3*i+0] ]);
				glTexCoord2f(0.0, 0.0);
				glVertex3d(CurVertX[ CurIdx[3*i+0] ], CurVertY[ CurIdx[3*i+0] ], CurVertZ[ CurIdx[3*i+0] ]);
				
				glColor3f( RedVert[CurIdx[3*i+1]], GrnVert[	CurIdx[3*i+1] ], BluVert[ CurIdx[3*i+1] ] );
				//glNormal3d(CurNormX[ CurIdx[3*i+1] ], CurNormY[ CurIdx[3*i+1] ], CurNormZ[ CurIdx[3*i+1] ]);
				glTexCoord2f(1.0, 1.0);
				glVertex3d(CurVertX[ CurIdx[3*i+1] ], CurVertY[ CurIdx[3*i+1] ], CurVertZ[ CurIdx[3*i+1] ]);
				
				glColor3f( RedVert[CurIdx[3*i+2]], GrnVert[	CurIdx[3*i+2] ], BluVert[ CurIdx[3*i+2] ] );
				//glNormal3d(CurNormX[ CurIdx[3*i+2] ], CurNormY[ CurIdx[3*i+2] ], CurNormZ[ CurIdx[3*i+2] ]);
				glTexCoord2f(0.0, 1.0);
				glVertex3d(CurVertX[ CurIdx[3*i+2] ], CurVertY[ CurIdx[3*i+2] ], CurVertZ[ CurIdx[3*i+2] ]);
			glEnd();
	
		}
		glDisable(GL_TEXTURE_2D);

	}
	else
	{
		for( i = 0; i < numFaces; i++ )	{
			//glColor3f( i/double(numFaces), gseed, bseed);
			glBegin(GL_TRIANGLES);
				glColor3f( RedVert[CurIdx[3*i+0]], GrnVert[	CurIdx[3*i+0] ], BluVert[ CurIdx[3*i+0] ] );
				glNormal3d(CurNormX[ CurIdx[3*i+0] ], CurNormY[ CurIdx[3*i+0] ], CurNormZ[ CurIdx[3*i+0] ]);
				glVertex3d(CurVertX[ CurIdx[3*i+0] ], CurVertY[ CurIdx[3*i+0] ], CurVertZ[ CurIdx[3*i+0] ]);
				glColor3f( RedVert[CurIdx[3*i+1]], GrnVert[	CurIdx[3*i+1] ], BluVert[ CurIdx[3*i+1] ] );
				glNormal3d(CurNormX[ CurIdx[3*i+1] ], CurNormY[ CurIdx[3*i+1] ], CurNormZ[ CurIdx[3*i+1] ]);
				glVertex3d(CurVertX[ CurIdx[3*i+1] ], CurVertY[ CurIdx[3*i+1] ], CurVertZ[ CurIdx[3*i+1] ]);
				glColor3f( RedVert[CurIdx[3*i+2]], GrnVert[	CurIdx[3*i+2] ], BluVert[ CurIdx[3*i+2] ] );
				glNormal3d(CurNormX[ CurIdx[3*i+2] ], CurNormY[ CurIdx[3*i+2] ], CurNormZ[ CurIdx[3*i+2] ]);
				glVertex3d(CurVertX[ CurIdx[3*i+2] ], CurVertY[ CurIdx[3*i+2] ], CurVertZ[ CurIdx[3*i+2] ]);
			glEnd();

		}
	}
	glPopMatrix();
	glFinish();
	
}