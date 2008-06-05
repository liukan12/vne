#include "VNEObject.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "VNEObjList.h"
#include "WorldForce.h"
#include <iostream>
using namespace std;

VNEObjList::VNEObjList( VNEObject *firstObj )
{
	this->list.push_back(firstObj);
	this->length=list.size();
}

int VNEObjList::DoSelection() // return idx of selected object
{
	int iObjIdx = 0; // which obj has most hits?

	for(unsigned int i=0; i < this->list.size();i++)
	{
		glLoadName(i+1);
		this->list[i]->DrawSelf();
	}
	return iObjIdx; // TODO: this is currently only printing inside DrawSelf(), not returning !!

}

int VNEObjList::AddObj( VNEObject *newObj )
{
	this->list.push_back(newObj);
	this->length=list.size();
	return 0;

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

	if(index < this->list.size() )
	{
		return this->list[index];
	}
	else
	{
		cout<<"you ass, "<<index<<" is an invalid index!\n";
		return 0;
	}
}


int VNEObjList::TimeStepAll( )
{
	for(unsigned int i=0; i<this->list.size();i++)
	{
		this->list[i]->IncrementTime();
	}
	return 0;

}

int VNEObjList::DrawAll( )
{
	for(unsigned int i=0; i < this->list.size();i++)
	{
		this->list[i]->DrawSelf();
	}
	return 0;
}

void VNEObjList::PrintAll()
{
	for(unsigned int i=0; i < this->list.size();i++)
	{
		this->list[i]->PrintSelf();
	}

}