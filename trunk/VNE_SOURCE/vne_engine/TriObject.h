#ifndef TRIOBJECT
#define TRIOBJECT

#include "DrawObject.h"

class TriObject : public DrawObject
{
private: 

protected:

public:
	TriObject(const DrawObject& obj);
	TriObject& operator=(const DrawObject& rhs);
	void CopyObj(const DrawObject& obj);

	TriObject( const valarray<double> &NX, const valarray<double> &NY, const valarray<double> &NZ,
				const valarray<double> &VX,const valarray<double> &VY,const valarray<double> &VZ,
				const valarray<int> &IDX);
	
	~TriObject( ); // destructor
	virtual void DrawSelf();
	virtual void PrintSelf() {}; 
	
};


#endif

