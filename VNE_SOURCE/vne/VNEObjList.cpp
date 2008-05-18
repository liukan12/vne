#include "VNEObject.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "VNEObjList.h"

using namespace std;

VNEObjList::VNEObjList( VNEObject *firstObj )
{
	this->firstNode = new ObjNode();
	this->firstNode->bIsFirst = true;
	this->firstNode->bIsLast = true;
	this->firstNode->curObj = firstObj;
	
	this->lastNode = this->firstNode;


}

int VNEObjList::AddObj( VNEObject *newObj )
{
int iRet = 0;
	
	ObjNode* newNode = new ObjNode();
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

	bool bContinue = true;
	ObjNode* tempNode = this->firstNode;

	while( bContinue )
	{
		tempNode->curObj->IncrementTime();
			
		if( tempNode->bIsLast )
			bContinue = false;
		else
			tempNode = tempNode->nextNode;
	}

	return iRet;
}

int VNEObjList::DrawAll( )
{
int iRet = 0;

	ObjNode* tempNode = this->firstNode;
	bool bContinue = true;
	
	while( bContinue )
	{
		iRet = tempNode->curObj->DrawSelf();
		if( iRet != 0 )
			return iRet;
		if( tempNode->bIsLast )
			bContinue = false;
		else
			tempNode = tempNode->nextNode;
	}

	return iRet;
}

void VNEObjList::PrintAll()
{
	ObjNode* tempNode = this->firstNode;
	bool bContinue = true;
	
	while( bContinue )
	{
		tempNode->curObj->PrintSelf();
		if( tempNode->bIsLast )
			bContinue = false;
		else
			tempNode = tempNode->nextNode;
	}

}