#ifndef CAMERACONTROL
#define CAMERACONTROL

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

class CameraControl
{
public:
	CameraControl( );
	CameraControl(double ex, double ey, double ez, double zmin, double zmax);
	~CameraControl();
	void GetEyeLocs( double* ex, double* ey, double* ez );
	void GetEyeDirs( double* dx, double* ey, double* ez );
	void RotateLocal( double x, double y, double z, double theta );
	void Detach() { bIsAttached = false; }
	void ResetPosition( );
	void TranslateBy( double dx, double dy, double dz );
	void TranslateTo( double x, double y, double z );
	void PointAt( double x, double y, double z );
	void ResizeCallbackHandler( int w = -1, int h = -1 );
	void UpdateAttachedCamera();
	void GetWH( int* w, int* h );
	void ModelView();
	
private:
	bool bIsAttached;
	int iWindowW;
	int iWindowH;
	double eyex, eyey, eyez; //eye location
	double atx, aty, atz; // pointing eye towards
	double initx, inity, initz; // where we started, if we need to reset
	double zmin;
	double zmax;

};

#endif