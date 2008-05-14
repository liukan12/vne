#include "CVector.h"
#include "CMatrix.h"

#include <iostream>
#include <stdlib.h>
using std::cout;

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

void CVector::PrintSelf()
{
	cout<<"Printing Vector of length "<<this->Length()<<"\n[ ";

	for( int i = 0; i < this->Length(); i++ )
	{
	double dx;
	this->GetValueAt(i, &dx );
		cout<<dx<<"\t";
	}
	cout<<" ]\n";

}

int  CVector::MultByMatrix( CMatrix* mtrx )
{
	// WARNING: CURRENTLY ONLY DOES SQUARE MATRICES!!!

	// linear transformation from R^n to R^m , m & n finite
	int i,j,n,m;
	mtrx->GetSize(&m, &n );
	if( this->Length() != n )
		return 1; // number of columns in matrix doesn't equal length of vector
	
	double dTemp1 = 0.0;
	double dTemp2 = 0.0;
	double dTemp3 = 0.0;
	CVector tempVec(3);
	for( i = 0; i < n; i++ )
	{
		dTemp2= 0.0;
		for( j = 0; j < m; j++ )
		{
			this->GetValueAt( j, &dTemp1 );
			dTemp3 = mtrx->GetValueAt(i,j);
			dTemp2 += dTemp1 * dTemp3;
		}
		tempVec.SetValueAt(i,dTemp2);
	}
	
	for( i = 0; i < n; i++ )
	{
		tempVec.GetValueAt(i,&dTemp1);
		this->SetValueAt(i,dTemp1);
	}
	
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