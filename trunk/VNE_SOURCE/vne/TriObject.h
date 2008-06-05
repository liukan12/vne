#ifndef TRIOBJECT
#define TRIOBJECT

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <valarray>
#include <numeric>
#include "DrawObject.h"

using namespace std;

class TriObject : public DrawObject
{
public: 

	string objName;

public:
	TriObject(const TriObject& obj);
	//TriObject& operator=(const TriObject& rhs);
	//void CopyObj(const TriObject& obj);

	TriObject(const valarray<double> NX, const valarray<double> NY, const valarray<double> NZ,
				const valarray<double> VX,const valarray<double> VY,const valarray<double> VZ,
				const valarray<int> IDX);
	
	~TriObject( ); // destructor
	virtual void setTexture(string fileName);
	virtual void setTexture(VNETexture* newTex);
	virtual void DrawSelf();
	virtual void PrintSelf() {}; 
	
};


#endif

