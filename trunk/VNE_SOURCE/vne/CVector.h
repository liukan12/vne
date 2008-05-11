#ifndef CVECTOR
#define CVECTOR

#include "CArray.h"
#include "CMatrix.h"

class CVector
{
protected:
	CDoubleArray* m_cdaData;
public:
	CVector( int iLength );
	CVector();
	CVector( const CVector& cv );
	~CVector();
	bool GetValueAt(int iIndex, double* output);
	bool SetValueAt(int iIndex, double value);
	bool Resize(int iNewSize);
	int  MultByMatrix( CMatrix* mtrx );
	double Max();
	int  Length();
};

bool TestVectorTest();

#endif
