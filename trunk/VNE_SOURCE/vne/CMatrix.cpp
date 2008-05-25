#include "CMatrix.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

#define min(a,b) (a < b) ? a : b

CMatrix::CMatrix(int iRows, int iCols)
{
	m_ppdData = new double*[iRows];
	int i,j;
	for( i = 0; i < iRows; i++ ) {
		m_ppdData[i] = new double[iCols];
		for( j = 0; j < iCols; j++ )
			m_ppdData[i][j] = 0.0;
	}
	m_iCols = iCols;
	m_iRows = iRows;

}

CMatrix::~CMatrix()
{
	int i;
	for( i = 0; i < m_iRows; i++ )
		delete [] m_ppdData[i];
	delete [] m_ppdData;
	//delete m_ppdData;
	//delete m_ppdData;
}

int CMatrix::Resize( int iRows, int iCols )
{
	if( iCols < 1 ) return 1;
	if( iRows < 1 ) return 2;
	
	double** ppdTemp = new double*[m_iRows];
	int i,j;
	for( i = 0; i < m_iRows; i++ ) {
		ppdTemp[i] = new double[m_iCols];
		for( j = 0; j < m_iCols; j++ )
			ppdTemp[i][j] = m_ppdData[i][j];
	}

	for( i = 0; i < m_iRows; i++ )
		delete m_ppdData[i];
	delete m_ppdData;

	int prevCols = m_iCols;
	int prevRows = m_iRows;
	m_iCols = iCols;
	m_iRows = iRows;

	m_ppdData = new double*[m_iRows];
	for( i = 0; i < m_iRows; i++ ) {
		m_ppdData[i] = new double[m_iCols];
		for( j = 0; j < m_iCols; j++ )
			m_ppdData[i][j] = 0.0;
	}
	for( i = 0; i < min(prevRows, m_iRows); i++ ) {
		for( j = 0; j < min(prevCols, m_iCols); j++ ) {
			m_ppdData[i][j] = ppdTemp[i][j];
		}
	}

	for( i = 0; i < prevRows; i++ )
		delete ppdTemp[i];
	delete ppdTemp;

	return 0;
}

double CMatrix::GetValueAt( int iRow, int iCol )
{
	double eps = 1e30;
	if( iCol < 0 ) return -1/eps;
	if( iRow < 0 ) return -1/eps;
	if( iRow > (m_iRows - 1) ) return -1/eps;
	if( iCol > (m_iCols - 1) ) return -1/eps;
	return m_ppdData[iRow][iCol];
}

int CMatrix::SetValueAt( int iRow, int iCol, double input )
{
	if( iCol < 0 ) return 1;
	if( iRow < 0 ) return 2;
	if( iRow > (m_iRows - 1) ) return 3;
	if( iCol > (m_iCols - 1) ) return 4;
	m_ppdData[iRow][iCol] = input;
	return 0;
}

void CMatrix::GetSize( int* rowOut, int* colOut )
{
	(*colOut) = this->m_iCols;
	(*rowOut) = this->m_iRows;
}

void CMatrix::PrintSelf()
{
	int i,j;
	for( i = 0; i < this->m_iRows; i++ )
	{
		for( j = 0; j < this->m_iCols; j++ )
		{
			double dTemp = this->GetValueAt(i,j);
			//cout<<dTemp<<" ";
		}
		//cout<<"\n";
	}
}

bool TestMatrixTest()
{
	int k;
	int iRet = 0;
	int i,j;
	
	for (k = 0; k < 100; k++ )
	{
		CMatrix* mtrx = new CMatrix(2048,2048*2);
		mtrx->GetSize(&i,&j);
		iRet = iRet + int( i != 2048 ) + int( j != 2048*2 );
		mtrx->SetValueAt(0,0,1.0);
		mtrx->Resize( 2047, 2047 );
		//mtrx->GetValueAt(0,0,&dVal);
		//iRet = iRet + ( dVal != 1.0 );
		mtrx->~CMatrix();
	}
	return (iRet == 0);
}