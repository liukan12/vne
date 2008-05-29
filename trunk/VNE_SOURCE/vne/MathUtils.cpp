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
#include "CVector.h"

using namespace std;

// WARNING: There is probably a math library we should be using to do these
// that is highly optimized (perhaps OpenCV, intel math kernel, etc ?? )

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

double VecNorm(CVector* vec1, CVector* vec2)
{
	double x1,y1,z1;
	double x2,y2,z2;
	vec1->GetValueAt(&x1,&y1,&z1);
	vec2->GetValueAt(&x2,&y2,&z2);
	return ( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
}

double DotProd( CVector* vec1, CVector* vec2 )
{
	double x1,y1,z1;
	double x2,y2,z2;
	vec1->GetValueAt(&x1,&y1,&z1);
	vec2->GetValueAt(&x2,&y2,&z2);
	return ( x1*x2 + y1*y2 + z1*z2 );
}			