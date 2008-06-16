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
#include "DrawObject.h"
#include "IOUtils.h"
#include <valarray>
#include <numeric>
#include "VNETexture.h"

using namespace std;

class DrawObject
{
public: 

	string objName;

protected:
	valarray<double> CurVertX; // current vertices of triangle faces w.r.t. local coordinate system
	valarray<double> CurVertY; // current vertices of triangle faces w.r.t. local coordinate system
	valarray<double> CurVertZ; // current vertices of triangle faces w.r.t. local coordinate system
	
	int numFaces;
	int numVerts;
	valarray<double> CurNormX; // normals at vertices
	valarray<double> CurNormY; // normals at vertices
	valarray<double> CurNormZ; // normals at vertices
	valarray<int>	 CurIdx; // indices into Verts & Norms
	
	VNETexture* objTexture;
	double rseed, gseed, bseed; // RGB seeds for coloring the vertices
	double colorVariance;
	bool bHasTexture;


public:
	//DrawObject(const DrawObject& obj);
	//DrawObject& operator=(const DrawObject& rhs);
	//void CopyObj(const DrawObject& obj);

	DrawObject( const valarray<double> &NX, const valarray<double> &NY, const valarray<double> &NZ,
				const valarray<double> &VX,const valarray<double> &VY,const valarray<double> &VZ,
				const valarray<int> &IDX);
	
	~DrawObject( ); // destructor
	virtual void setTexture(string fileName) {};
	virtual void setTexture(VNETexture* newTex) {};
	virtual void DrawSelf() {};
	virtual void PrintSelf() {}; 
	virtual void UpdateData( const valarray<double> &NX, const valarray<double> &NY, const valarray<double> &NZ,
				const valarray<double> &VX,const valarray<double> &VY,const valarray<double> &VZ,
				const valarray<int> &IDX);
	
};


#endif

