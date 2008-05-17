#include "VNEObject.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "VNEObjList.h"

using namespace std;

VNEObjList::VNEObjList( VNEObject *firstObj )
{
	this->firstNode->curObj = firstObj;
	this->firstNode->bIsFirst = true;
	this->firstNode->bIsLast = true;

}

int VNEObjList::AddObj( VNEObject *newObj )
{
int iRet = 0;
	
	ObjNode* newNode;
	newNode->curObj = newObj;
	this->lastNode->nextNode = newNode;

	ObjNode* tempNode = this->lastNode;

	this->lastNode->bIsLast = false;
	this->lastNode = newNode;
	this->lastNode->bIsLast = true;
	this->lastNode->prevNode = tempNode;

	return iRet;
}

int VNEObjList::TimeStepAll( )
{
int iRet = 0;

	return iRet;
}

int VNEObjList::DrawAll( )
{
int iRet = 0;

	return iRet;
}