#ifndef ROTATIONMATRIX_H
#define ROTATIONMATRIX_H

#include "CMatrix.h"
#include "CVector.h"

class RotationMatrix : public CMatrix
{
public:
	RotationMatrix( CVector* direction, double dAngle );


};


#endif