#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

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