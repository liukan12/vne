#include "CVector.h"
#include "CMatrix.h"

CVector::CVector(int iLength)
{
	m_cdaData = new CDoubleArray(iLength);
}
CVector::CVector()
{
	m_cdaData = new CDoubleArray( );
} 

CVector::~CVector()
{
	m_cdaData->~CDoubleArray();
}

int  CVector::MultByMatrix( CMatrix* mtrx )
{
	//int i,j;
	//mtrx->GetSize(&i, &j );
	//if( this->Length() != j )
	//	return 1; // number of columns in matrix doesn't equal length of vector
	//
	return 0;
}

bool CVector::GetValueAt(int iIndex, double* output)
{
	return m_cdaData->GetValueAt(iIndex, output);
}
bool CVector::SetValueAt(int iIndex, double value)
{
	return this->m_cdaData->SetValueAt( iIndex, value );
}

/*
bool CVector::Resize(int iNewSize)
{
	return this->m_cdaData->Resize( iNewSize );
}*/
int CVector::Length()
{
	return this->m_cdaData->Length();
}

double CVector::Max()
{
	return m_cdaData->Max();
}