#include "VNEObject.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "VNEObjList.h"
#include "WorldForce.h"
#include <iostream>
//#include "stl/vector.h"
using namespace std;

VNEObjList::VNEObjList( VNEObject *firstObj )
{
	this->list.push_back(firstObj);
	this->length=list.size();
	 /*this->firstNode = new ObjNode();
	this->firstNode->bIsFirst = true;
	this->firstNode->bIsLast = true;
	this->firstNode->curObj = firstObj;
	this->firstNode->myIndex=0;
	
	this->length=1;
	this->lastNode = this->firstNode;
	this->lastNode->myIndex=0;*/


}

int VNEObjList::DoSelection() // return idx of selected object
{
	int iObjIdx = 0; // which obj has most hits?

	for(unsigned int i=0; i < this->list.size();i++)
	{
		glLoadName(i+1);
		this->list[i]->DrawSelf();
	}
	return iObjIdx;

}

int VNEObjList::AddObj( VNEObject *newObj )
{
	
	this->list.push_back(newObj);
	this->length=list.size();
	return 0;
//int iRet = 0;
//	
//	ObjNode* newNode = new ObjNode();
//	newNode->curObj = newObj;
//	this->lastNode->nextNode = newNode;
//
//	ObjNode* tempNode = this->lastNode;
//	
//	this->lastNode->bIsLast = false;
//	this->lastNode = newNode;
//	this->lastNode->bIsLast = true;
//	this->lastNode->prevNode = tempNode;
//	this->lastNode->myIndex=this->Length();
//	this->length++;
//
//	return iRet;
}
void VNEObjList::DeleteObjAt(int index)
{
	if(!list.empty() && index > 0 && index < list.size())
	{
	this->list.erase(list.begin()+index);
	this->length=list.size();
	}
}
VNEObject* VNEObjList::GetObjectAt(int index)
{

	if(index < this->list.size() ){
		return this->list[index];
	}
	else
	{
		cout<<"you ass, "<<index<<" is an invalid index!\n";
		return 0;
	}

	/*
	if(index<0||index >this->length)
	{
		cout<<"you ass, "<<index<<" is an invalid index!\n";
		return 0;


	}*/
	
	//	
	//ObjNode* tempNode = this->firstNode;
	//int counter=0;
	//while(counter<this->length)
	//{
	//	if(tempNode->myIndex==index)
	//		return tempNode->curObj;

	//	
	//	counter++;
	//	if(!(tempNode->bIsLast))
	//	{
	//		tempNode = tempNode->nextNode;
	//	}
	//}	
	//cout<<"our fault, "<<index<<" is an invalid index!\n";
	//	return 0;
}


void VNEObjList::AccelAll( WorldForce* force )
{
	for(unsigned int i=0; i<this->list.size();i++)
	{
		force->AccelerateObject(this->list[i]);
	}
/*	bool bContinue = true;
	ObjNode* tempNode = this->firstNode;

	while( bContinue )
	{
		force->AccelerateObject(tempNode->curObj);
			
		if( tempNode->bIsLast )
			bContinue = false;
		else
			tempNode = tempNode->nextNode;
	}*/	
}

int VNEObjList::TimeStepAll( )
{
	for(unsigned int i=0; i<this->list.size();i++)
	{
		this->list[i]->IncrementTime();
	}
	return 0;
//int iRet = 0;
//
//	bool bContinue = true;
//	ObjNode* tempNode = this->firstNode;
//
//	while( bContinue )
//	{
//		tempNode->curObj->IncrementTime();
//			
//		if( tempNode->bIsLast )
//			bContinue = false;
//		else
//			tempNode = tempNode->nextNode;
//	}
//
//	return iRet;
}

int VNEObjList::DrawAll( )
{
	for(unsigned int i=0; i < this->list.size();i++)
	{
		this->list[i]->DrawSelf();
	}
	return 0;
	
//int iRet = 0;
//
//	ObjNode* tempNode = this->firstNode;
//	bool bContinue = true;
//	
//	while( bContinue )
//	{
//		iRet = tempNode->curObj->DrawSelf();
//		if( iRet != 0 )
//			return iRet;
//		if( tempNode->bIsLast )
//			bContinue = false;
//		else
//			tempNode = tempNode->nextNode;
//	}
//
//	return iRet;
}

void VNEObjList::PrintAll()
{
	for(unsigned int i=0; i < this->list.size();i++)
	{
		this->list[i]->PrintSelf();
	}
	/*ObjNode* tempNode = this->firstNode;
	bool bContinue = true;
	
	while( bContinue )
	{
		tempNode->curObj->PrintSelf();
		if( tempNode->bIsLast )
			bContinue = false;
		else
			tempNode = tempNode->nextNode;
	}*/

}