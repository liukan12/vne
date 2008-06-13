#ifndef DRAWOBJECT
#define DRAWOBJECT

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <valarray>
#include <numeric>
#include "VNETexture.h"

using namespace std;

class DrawObject
{
public: 
	string objName;

protected:
	valarray<double> CurVertX; 
	valarray<double> CurVertY; 
	valarray<double> CurVertZ;
	
	int numFaces;
	int numVerts;
	valarray<double> CurNormX;	// normals at vertices
	valarray<double> CurNormY;	// normals at vertices
	valarray<double> CurNormZ;	// normals at vertices
	valarray<int>	 CurIdx;	// indices into Verts & Norms
	
	bool bHasTexture;
	VNETexture* objTexture;

	double rseed, gseed, bseed; // RGB seeds for coloring the vertices
	valarray<double> RedVert;
	valarray<double> BluVert;
	valarray<double> GrnVert;

public:
	DrawObject();
	DrawObject(const DrawObject& obj);
	DrawObject& operator=(const DrawObject& rhs);
	void CopyObj(const DrawObject& obj);

	DrawObject( const valarray<double> &NX, const valarray<double> &NY, const valarray<double> &NZ,
				const valarray<double> &VX,const valarray<double> &VY,const valarray<double> &VZ,
				const valarray<int> &IDX);
	
	~DrawObject( ); // destructor
	virtual void setTexture(string fileName);
	virtual void setTexture(VNETexture* newTex) {};
	virtual void DrawSelf() {};
	virtual void PrintSelf() {}; 
	virtual void UpdateData( const valarray<double> &NX, const valarray<double> &NY, const valarray<double> &NZ,
				const valarray<double> &VX,const valarray<double> &VY,const valarray<double> &VZ,
				const valarray<int> &IDX);
	virtual void SetColorSeed( double r, double g, double b) { rseed = r; gseed = g; bseed = b; }
	virtual void SetColors( const valarray<double> &Red, const valarray<double> &Green, const valarray<double> &Blue );
};


#endif

