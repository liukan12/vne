#include "DrawObject.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <valarray>
#include <numeric>

DrawObject::DrawObject( const valarray<double> &NX, const valarray<double> &NY, const valarray<double> &NZ,
				const valarray<double> &VX,const valarray<double> &VY,const valarray<double> &VZ,
				const valarray<int> &IDX)
{
	CurVertX = VX; 
	CurVertY = VY; 
	CurVertZ = VZ; 
	CurNormX = NX; 
	CurNormY = NY; 
	CurNormZ = NZ; 
	CurIdx = IDX;

	this->bHasTexture=false;

}

void DrawObject::UpdateData( const valarray<double> &NX, const valarray<double> &NY, const valarray<double> &NZ,
				const valarray<double> &VX,const valarray<double> &VY,const valarray<double> &VZ,
				const valarray<int> &IDX)
{

	CurVertX = VX; 
	CurVertY = VY; 
	CurVertZ = VZ; 
	CurNormX = NX; 
	CurNormY = NY; 
	CurNormZ = NZ; 
	CurIdx = IDX;

}

//DrawObject::DrawObject(const DrawObject& obj)
//{
//	
//}

//DrawObject& operator=(const DrawObject& rhs)
//{
//	return 0;
//}

//void CopyObj(const DrawObject& obj)
//{
//
//}
	
DrawObject::~DrawObject( )
{
	delete objTexture;
}