#include "CArray.h"
#include "Math.h"

void CDoubleArray::CopyObj(const CDoubleArray& obj)
{
	//delete this->m_data;
	m_iLength=obj.m_iLength;
	m_data=obj.m_data;
	
}
CDoubleArray::CDoubleArray(const CDoubleArray& obj) 
{
	CopyObj(obj);
}
CDoubleArray& CDoubleArray::operator=(const CDoubleArray& rhs) 
{
	CopyObj(rhs);
	return *this;
}


CDoubleArray::CDoubleArray(int iLength)
{
	  m_iLength = iLength;
      m_data = new double[iLength];
	  int i;
	  for( i = 0; i < iLength; i++ )
		  m_data[i] = 0.0;
}


CDoubleArray::~CDoubleArray()
{
	delete [] m_data;
	//delete m_iLength;
}

int CDoubleArray::Length()
{
	return m_iLength;
}

bool CDoubleArray::GetValueAt(int iIndex, double* output)
{
	if(iIndex >= m_iLength)
		return 1;
	else
		(*output) = m_data[iIndex];
	return 0;
}

bool CDoubleArray::SetValueAt(int iIndex, double value)
{
	if(iIndex >= m_iLength)
		return 1;
	else
		m_data[iIndex] = value;
	return 0;
}

/* temporarily commented out; memcpy is C command that doesn't work here for some reason
   (this code was ported from another project)
   Just replace it with a for loop that copies each value in m_data to tempArray
   when get around to it...
bool CDoubleArray::Resize(int iNewSize)
{
	if( iNewSize < 1 )
		return 1;
	double* tempArray = new double[m_iLength];
	memcpy(tempArray, m_data, m_iLength * sizeof(double) );
	delete [] m_data;
	m_data = new double[iNewSize];
	int iOldSize = m_iLength;
	m_iLength = iNewSize;
	int i; 
	for( i = 0; i < min(iNewSize,iOldSize); i++ )
		m_data[i] = tempArray[i];
	delete [] tempArray;
	return 0;
} */

double CDoubleArray::Max()
{
	int i;
	double max = 0.0;
	for( i = 0; i < m_iLength; i++ )
	{
		if( abs(m_data[i]) > max )
			max = abs(m_data[i]);
	}
	return max;
}

int ArraySelfTest()
{ // this test is probably broken since memcpy doesn't work for some reason (link issues)
	int iReturn = 0;

	CDoubleArray cda(5);
	double val;
	cda.GetValueAt(1,&val);
	iReturn += ( val != 0.0 );
	cda.SetValueAt(3,5.0);
	cda.GetValueAt(3,&val);
	iReturn += ( val != 5.0 );
	//cda.Resize(1);
	cda.SetValueAt(0,3.1415);
	cda.GetValueAt(0,&val);
	iReturn += ( val != 3.1415);
	iReturn += (cda.Length() != 1);

	return iReturn;

}