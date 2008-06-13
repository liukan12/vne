#include "DrawObject.h"

DrawObject::DrawObject()
{

}

DrawObject::DrawObject( const valarray<double> &NX, const valarray<double> &NY, const valarray<double> &NZ,
				const valarray<double> &VX,const valarray<double> &VY,const valarray<double> &VZ,
				const valarray<int> &IDX)
{
	this->CurIdx = IDX;
	this->CurVertX = VX;
	this->CurVertY = VY;
	this->CurVertZ = VZ;
	this->CurNormX = NX;
	this->CurNormY = NY;
	this->CurNormZ = NZ;
	rseed = 1.0;
	bseed = 0.5;
	gseed = 0.5;
	this->bHasTexture = false;
}

DrawObject::~DrawObject()
{
	delete objTexture;
}

void DrawObject::UpdateData( const valarray<double> &NX, const valarray<double> &NY, const valarray<double> &NZ,
				const valarray<double> &VX,const valarray<double> &VY,const valarray<double> &VZ,
				const valarray<int> &IDX)
{
	this->CurIdx = IDX;
	this->CurVertX = VX;
	this->CurVertY = VY;
	this->CurVertZ = VZ;
	this->CurNormX = NX;
	this->CurNormY = NY;
	this->CurNormZ = NZ;
}

void DrawObject::setTexture(string fileName)
{
	this->objTexture=new VNETexture(fileName);
	this->bHasTexture=true;
}

void DrawObject::SetColors( const valarray<double> &Red, const valarray<double> &Green, const valarray<double> &Blue )
{
	RedVert = Red;
	BluVert = Blue;
	GrnVert = Green;
}