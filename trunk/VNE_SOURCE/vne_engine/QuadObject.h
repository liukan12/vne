#ifndef QUADOBJECT
#define QUADOBJECT

#include "DrawObject.h"

class QuadObject : public DrawObject
{
private: 

protected:

public:
	QuadObject(const DrawObject& obj);
	QuadObject& operator=(const DrawObject& rhs);
	void CopyObj(const DrawObject& obj);

	QuadObject( const valarray<double> &NX, const valarray<double> &NY, const valarray<double> &NZ,
				const valarray<double> &VX,const valarray<double> &VY,const valarray<double> &VZ,
				const valarray<int> &IDX);
	
	~QuadObject( ); // destructor
	virtual void DrawSelf();
	virtual void PrintSelf() {}; 
	
};


#endif

