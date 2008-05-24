#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "MathUtils.h"

using namespace std;



void Normalize( double* mx, double* my, double* mz )
{
	double norm = sqrt( *mx**mx + *my**my + *mz**mz );
	if( norm > 1e-12 )
	{
		*mx /= norm;
		*my /= norm;
		*mz /= norm;
	}
	else
		cout<<"error, zero length vector being normalized \n";
}

//void CreateCheckerTex()
//{
//	int i,j,c;
//	for( i = 0; i < CheckerTexSize; i++ ) {
//		for( j = 0; j < CheckerTexSize; j++) {
//			c = ((((i&0x8)==0)^((j&0x8))==0))*255;
//				CheckerTex[i][j][0] = (GLubyte) c;
//				CheckerTex[i][j][1] = (GLubyte) c;
//				CheckerTex[i][j][2] = (GLubyte) c;
//				CheckerTex[i][j][3] = (GLubyte) 255;
//		}
//	}
//}
			