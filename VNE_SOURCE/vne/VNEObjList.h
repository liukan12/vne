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

struct ObjNode
{
	int myIndex;
	VNEObject* curObj;
	ObjNode* nextNode;
	ObjNode* prevNode;
	bool bIsFirst;
	bool bIsLast;
};

class VNEObjList
{
private:
	int length;
	vector<VNEObject*> list;
	
public:
	void DeleteObjAt(int index);
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
	int Length() {return length;}
	VNEObject* GetObjectAt(int index);

};

#endif