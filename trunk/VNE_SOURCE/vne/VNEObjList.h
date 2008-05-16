#ifndef VNEOBJLIST_H
#define VNEOBJLIST_H

#include "VNEObject.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>

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
	ObjNode* firstNode;
	ObjNode* lastNode;
public:
	VNEObjList( );
	VNEObjList( VNEObject* firstObj );
	int ClearAll();
	int DrawAll();
	int TimeStepAll();
	int DeleteObj( string objName );
	int AddObj( VNEObject* newObj );

};

#endif