#ifndef CMATRIX
#define CMATRIX


class CMatrix
{
private:
	double** m_ppdData;
	int m_iCols;
	int m_iRows;
public:
	CMatrix(int iRows, int iCols);
	CMatrix( CMatrix* other );
	~CMatrix();
	int Resize( int iRows, int iCols );
	int GetValueAt( int iRow, int iCol, double* output );
	int SetValueAt( int iRow, int iCol, double input );
	void GetSize( int* rowOut, int* colOut );

};

bool TestMatrixTest();

#endif