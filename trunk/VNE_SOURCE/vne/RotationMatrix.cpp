#include "RotationMatrix.h"
#include <math.h>

// THIS IS DEPRECATED DUE TO PERFORMANCE !!!!!!!!!!!!

RotationMatrix::RotationMatrix( CVector *direction, double dAngle ) : CMatrix(3,3)
{
	//CMatrix(3,3);
	double x,y,z;
	direction->GetValueAt(0,&x);
	direction->GetValueAt(1,&y);
	direction->GetValueAt(2,&z);

	double dS = sin( dAngle );
	double dC = cos( dAngle );
	double c1 = 1 - dC;

	SetValueAt(0,0, x*x*c1+dC);
	SetValueAt(0,1, x*y*c1 - z*dS);
	SetValueAt(0,2, x*z*c1+y*dS);
	SetValueAt(1,0, x*y*c1+z*dS);
	SetValueAt(1,1, y*y*c1+dC);
	SetValueAt(1,2, y*z*c1-x*dS);
	SetValueAt(2,0, x*z*c1-y*dS);
	SetValueAt(2,1, y*z*c1+x*dS);
	SetValueAt(2,2, z*z*c1+dC);

}

RotationMatrix::~RotationMatrix()
{

}