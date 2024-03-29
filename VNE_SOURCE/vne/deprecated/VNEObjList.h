#ifndef VNEOBJLIST_H
#define VNEOBJLIST_H
#include <vector>



#include "VNEObject.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "WorldForce.h"

//using namespace cliext;
using namespace std; // note for the weary: this is required so that "string" is recognized from std::string

class VNEObjList
{
private:
	int length;
	vector<VNEObject*> list;
	
public:
	void DeleteObjAt(int index);
	VNEObjList( );
	VNEObjList( VNEObject* firstObj );
	int DoSelection(); // return idx of selected object
	int ClearAll();
	int DrawAll();
	int TimeStepAll();
	void PrintAll();
	void AccelAll( WorldForce* force );
	int DeleteObj( string objName );
	int AddObj( VNEObject* newObj );
	int Length() {return length;}
	VNEObject* GetObjectAt(int index);

};

#endif