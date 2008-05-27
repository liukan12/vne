#ifndef CMATRIX
#define CMATRIX


class CMatrix
{
private:
	double** m_ppdData;
	int m_iCols;
	int m_iRows;
public:
	CMatrix(const CMatrix& obj);
	CMatrix& operator=(const CMatrix& rhs);
	void CopyObj(const CMatrix& obj);

	CMatrix(int iRows, int iCols);
	CMatrix( CMatrix* other );
	~CMatrix();
	int Resize( int iRows, int iCols );
	double GetValueAt( int iRow, int iCol);
	int SetValueAt( int iRow, int iCol, double input );
	void GetSize( int* rowOut, int* colOut );
	void PrintSelf();
};

bool TestMatrixTest();

#endif