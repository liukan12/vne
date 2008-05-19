#ifndef VNEOBJLIST_H
#define VNEOBJLIST_H

#include "VNEObject.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "WorldForce.h"

using namespace std; // note for the weary: this is required so that "string" is recognized from std::string

struct ObjNode
{
	VNEObject* curObj;
	ObjNode* nextNode;
	ObjNode* prevNode;
	bool bIsFirst;
	bool bIsLast;
};

class VNEObjList
{
private:
	
public:
	ObjNode* firstNode;
	ObjNode* lastNode;
	VNEObjList( );
	VNEObjList( VNEObject* firstObj );
	int ClearAll();
	int DrawAll();
	int TimeStepAll();
	void PrintAll();
	void AccelAll( WorldForce* force );
	int DeleteObj( string objName );
	int AddObj( VNEObject* newObj );

};

#endif