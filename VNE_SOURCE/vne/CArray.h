#ifndef CDOUBLEARRAY
#define CDOUBLEARRAY

class CDoubleArray {



private:
	int m_iLength;
private:
	double* m_data;

  public:
    CDoubleArray(int iLength = 1);
	bool GetValueAt(int iIndex, double* output);
	bool SetValueAt(int iIndex, double value);
	//bool Resize(int iNewSize);
	int  Length();
	double Max();
	~CDoubleArray();
	CDoubleArray( const CDoubleArray& cda );

	
    
};

int ArraySelfTest();

#endif