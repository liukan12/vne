#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "VNEObject.h"

VNEObject::VNEObject()
{
// default: make a regular tetrahedron centered at global origin
	unitDrift1 = 45.0;

	CVector GlobalCentCoord(3);
	GlobalCentCoord.SetValueAt(0,0.0);
	GlobalCentCoord.SetValueAt(1,0.0);
	GlobalCentCoord.SetValueAt(2,0.0);

	CVector Velocity(3);
	Velocity.SetValueAt(0,0.0);
	Velocity.SetValueAt(0,0.0);
	Velocity.SetValueAt(0,0.0);

	this->numFaces = 4;

	// 0  1 0 | 0 1 0 | 0 0 0 | 1 0 0
 	// 0  0 1 | 0 0 0 | 0 1 0 | 0 1 0
	// 0  0 0 | 0 0 1 | 0 0 1 | 0 0 1

	RefTriVert = new CMatrix(3, 12); // each 3x3 block will define one triangle face
	RefTriVert->SetValueAt(0,0,0.0); RefTriVert->SetValueAt(1,0,0.0); RefTriVert->SetValueAt(2,0,0.0);
	RefTriVert->SetValueAt(0,1,1.0); RefTriVert->SetValueAt(1,1,0.0); RefTriVert->SetValueAt(2,1,0.0);
	RefTriVert->SetValueAt(0,2,0.0); RefTriVert->SetValueAt(1,2,1.0); RefTriVert->SetValueAt(2,2,0.0);

	RefTriVert->SetValueAt(0,3,0.0); RefTriVert->SetValueAt(1,3,0.0); RefTriVert->SetValueAt(2,3,0.0);
	RefTriVert->SetValueAt(0,4,1.0); RefTriVert->SetValueAt(1,4,0.0); RefTriVert->SetValueAt(2,4,0.0);
	RefTriVert->SetValueAt(0,5,0.0); RefTriVert->SetValueAt(1,5,0.0); RefTriVert->SetValueAt(2,5,1.0);

	RefTriVert->SetValueAt(0,6,0.0); RefTriVert->SetValueAt(1,6,0.0); RefTriVert->SetValueAt(2,6,0.0);
	RefTriVert->SetValueAt(0,7,0.0); RefTriVert->SetValueAt(1,7,1.0); RefTriVert->SetValueAt(2,7,0.0);
	RefTriVert->SetValueAt(0,8,0.0); RefTriVert->SetValueAt(1,8,0.0); RefTriVert->SetValueAt(2,8,1.0);

	RefTriVert->SetValueAt(0,9 ,1.0); RefTriVert->SetValueAt(1,9 ,0.0); RefTriVert->SetValueAt(2,9 ,0.0);
	RefTriVert->SetValueAt(0,10,0.0); RefTriVert->SetValueAt(1,10,1.0); RefTriVert->SetValueAt(2,10,0.0);
	RefTriVert->SetValueAt(0,11,0.0); RefTriVert->SetValueAt(1,11,0.0); RefTriVert->SetValueAt(2,11,1.0);

	CurTriVert = new CMatrix( 3, 12 );
	CurTriVert->SetValueAt(0,0,0.0); CurTriVert->SetValueAt(1,0,0.0); CurTriVert->SetValueAt(2,0,0.0);
	CurTriVert->SetValueAt(0,1,1.0); CurTriVert->SetValueAt(1,1,0.0); CurTriVert->SetValueAt(2,1,0.0);
	CurTriVert->SetValueAt(0,2,0.0); CurTriVert->SetValueAt(1,2,1.0); CurTriVert->SetValueAt(2,2,0.0);

	CurTriVert->SetValueAt(0,3,0.0); CurTriVert->SetValueAt(1,3,0.0); CurTriVert->SetValueAt(2,3,0.0);
	CurTriVert->SetValueAt(0,4,1.0); CurTriVert->SetValueAt(1,4,0.0); CurTriVert->SetValueAt(2,4,0.0);
	CurTriVert->SetValueAt(0,5,0.0); CurTriVert->SetValueAt(1,5,0.0); CurTriVert->SetValueAt(2,5,1.0);

	CurTriVert->SetValueAt(0,6,0.0); CurTriVert->SetValueAt(1,6,0.0); CurTriVert->SetValueAt(2,6,0.0);
	CurTriVert->SetValueAt(0,7,0.0); CurTriVert->SetValueAt(1,7,1.0); CurTriVert->SetValueAt(2,7,0.0);
	CurTriVert->SetValueAt(0,8,0.0); CurTriVert->SetValueAt(1,8,0.0); CurTriVert->SetValueAt(2,8,1.0);

	CurTriVert->SetValueAt(0,9 ,1.0); CurTriVert->SetValueAt(1,9 ,0.0); CurTriVert->SetValueAt(2,9 ,0.0);
	CurTriVert->SetValueAt(0,10,0.0); CurTriVert->SetValueAt(1,10,1.0); CurTriVert->SetValueAt(2,10,0.0);
	CurTriVert->SetValueAt(0,11,0.0); CurTriVert->SetValueAt(1,11,0.0); CurTriVert->SetValueAt(2,11,1.0);

}


int VNEObject::GetCurTriVerts(double *pdVerts, int *piIdx)
{
// return the vertices in OpenGL vertex array format
// [x0 y0 z0 x1 y1 z1 ... ]

	return 0;
}

int VNEObject::DrawSelf()
{
	int coordSize = this->numFaces * 9;
	int* idx = new int[numFaces * 3];
	double* verts = new double[coordSize];

	glColor3f(1.0, 0.5, 0.0 ); // TODO: make color an object and/or per-face property

	double dx, dy, dz;
	GlobalCentCoord.GetValueAt(0, &dx);
	GlobalCentCoord.GetValueAt(0, &dy);
	GlobalCentCoord.GetValueAt(0, &dz);

	int i;
	for( i = 0; i < coordSize; i+= 3 )
	{
		verts[i] = this->CurTriVert->GetValueAt(0,i/3);
		verts[i+1] = this->CurTriVert->GetValueAt(1,i/3);
		verts[i+2] = this->CurTriVert->GetValueAt(2,i/3);
		idx[i/3] = i/3;
	}
	////glEnableClientState(GL_VERTEX_ARRAY);
	////glVertexPointer(3, GL_DOUBLE, 0, verts);
	//glLoadIdentity();
	glPushMatrix();
	glTranslatef(dx, dy, dz);

	//glDrawElements(GL_TRIANGLES, coordSize/3, GL_INT, idx);
	glRotatef(this->unitDrift1,1.0,1.0,0.0);
	unitDrift1 += (rand()%7)/6.0 - 3.0;
	for( i = 0; i < numFaces; i++ )
	{
		glColor3f((rand()%7)/6.0,(rand()%7)/6.0,(rand()%7)/6.0);
		glBegin(GL_TRIANGLES);
			glVertex3d(verts[i*9+0+0], verts[i*9+0+1], verts[i*9+0+2]);
			glVertex3d(verts[i*9+3+0], verts[i*9+3+1], verts[i*9+3+2]);
			glVertex3d(verts[i*9+6+0], verts[i*9+6+1], verts[i*9+6+2]); 
			//glVertex3d(1.0,0.0,0.0);
			//glVertex3d(0.0,1.0,0.0);
			//glVertex3d(1.0,1.0,0.0);
		glEnd();
	}
	glPopMatrix();
	delete verts;
	delete idx;

	return 0;
}