#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <math.h>
#include "CMatrix.h"

using namespace std;

int ReadMeshData( CMatrix** TriVerts, string fileNameFaces, string fileNameVerts )
{
	// read a comma separted value file that defines the vertices
	// and indices into vertices for all faces

	vector<int> idx_array;
	char line[32];
	ifstream fin( fileNameFaces.c_str() );
	int iFaces = 0;
	while( !fin.eof() )
	{
		fin>>line;
		iFaces++;
		cout<<iFaces;
	}
	//iFaces--; // last line of file is blank
	fin.close();
	
	(*TriVerts) = new CMatrix(3, 3*iFaces);
	int* tempidx = new int[3 * iFaces];

	ifstream fin2( fileNameFaces.c_str() );
	
	string csTemp;
	cout<<"Reading face data...\n";
	cout<<"Creating object with "<<iFaces<<" triangles\n";
	int k = 0;
	int idx;
	while( !fin2.eof() )
	{
		fin2>>line;
		csTemp = line;
		//cout<<csTemp.c_str()<<"\n";
		int comma1_idx = csTemp.find_first_of(',');
		string s1 = csTemp.substr(0,comma1_idx);
		idx = atoi(s1.c_str());
		tempidx[ 3*k + 0 ] = idx;
		string rest1 = csTemp.substr(comma1_idx+1,csTemp.length());
		int comma2_idx = rest1.find_first_of(',');
		string s2 = rest1.substr(0,comma2_idx);
		idx = atoi(s2.c_str());
		tempidx[ 3*k + 1 ] = idx;
		string rest2 = rest1.substr(comma2_idx+1,rest1.length());
		idx = atoi(rest2.c_str());
		tempidx[ 3*k + 2 ] = idx;
		k++;
	}

	fin2.close();
	ifstream vin( fileNameVerts.c_str() );
	int iVerts = 0;
	k = 0;
	while( !vin.eof() )
	{
		vin>>line;
		iVerts++;
	}
	//iVerts--; // last line is blank
	vin.close();
	ifstream vin2( fileNameVerts.c_str() );
	double* tempverts = new double[3*iVerts];
	double val;
	k = 0;
	while( !vin2.eof() )
	{
		vin2>>line;
		csTemp = line;
		//cout<<csTemp.c_str()<<"\n";
		int comma1_idx = csTemp.find_first_of(',');
		string s1 = csTemp.substr(0,comma1_idx);
		val = atof(s1.c_str());
		tempverts[ 3*k + 0 ] = val;
		string rest1 = csTemp.substr(comma1_idx+1,csTemp.length());
		int comma2_idx = rest1.find_first_of(',');
		string s2 = rest1.substr(0,comma2_idx);
		val = atof(s2.c_str());
		tempverts[ 3*k + 1 ] = val;
		string rest2 = rest1.substr(comma2_idx+1,rest1.length());
		val = atof(rest2.c_str());
		tempverts[ 3*k + 2 ] = val;
		k++;
	}
	vin2.close();

	for( int i = 0; i < iFaces*3; i++ )
	{
		idx = tempidx[i];
		double xx = tempverts[(idx-1)*3+0]; // matlab counts starting with 1
		double yy = tempverts[(idx-1)*3+1];
		double zz = tempverts[(idx-1)*3+2];

		(*TriVerts)->SetValueAt(0,i,xx);
		(*TriVerts)->SetValueAt(1,i,yy);
		(*TriVerts)->SetValueAt(2,i,zz);

	}
	
	//(*TriVerts)->PrintSelf();

	delete [] tempverts;
	delete [] tempidx;
	iFaces--;  // this is a non-optimal way of handling end of line...
	return iFaces;
}