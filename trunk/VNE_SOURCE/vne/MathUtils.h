#ifndef MATHUTILS
#define MATHUTILS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "CVector.h"

void Normalize( double* mx, double* my, double* mz );

double VecNorm(CVector* vec1, CVector* vec2);

double DotProd( CVector* vec1, CVector* vec2 );

#endif