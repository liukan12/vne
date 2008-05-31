#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <valarray>
#include <numeric>

using namespace std;

int ReadMeshData( valarray<double> *TriVerts, valarray<double> *TriNorms, string fileNameFaces, string fileNameVerts, string fileNameNorms )
{
	// read a comma separted value file that defines the vertices
	// and indices into vertices for all faces

	char line[32];
	ifstream fin( fileNameFaces.c_str() );
	int iFaces = 0;
	while( !fin.eof() )
	{
		fin>>line;
		iFaces++;
		////cout<<iFaces;
	}
	//iFaces--; // last line of file is blank
	fin.close();
	
	*TriVerts = valarray<double>(3*3*(iFaces-1));
	*TriNorms = valarray<double>(3*3*(iFaces-1));
	
	int* tempidx = new int[3 * iFaces];

	ifstream fin2( fileNameFaces.c_str() );
	
	string csTemp;

	//cout<<"Reading face data...\n";
	//cout<<"Creating object with "<<iFaces<<" triangles\n";
	int k = 0;
	int idx;
	while( !fin2.eof() )
	{
		fin2>>line;
		csTemp = line;
		////cout<<csTemp.c_str()<<"\n";
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
	//cout<<"Counting vertices...\n";
	int iVerts = 0;
	k = 0;
	while( !vin.eof() )
	{
		vin>>line;
		iVerts++;
	}
	//iVerts--; // last line is blank
	vin.close();
	//cout<<"Done counting vertices \n";
	double* tempnorms = new double[3*iVerts];
	ifstream vin3( fileNameNorms.c_str() );
	//cout<<"Reading normals from file "<<fileNameNorms.c_str()<<"\n";
	double val;
	k = 0;
	while( !vin3.eof() )
	{
		vin3>>line;
		csTemp = line;
		//cout<<csTemp.c_str()<<"\n";
		int comma1_idx = csTemp.find_first_of(',');
		string s1 = csTemp.substr(0,comma1_idx);
		val = atof(s1.c_str());
		tempnorms[ 3*k + 0 ] = val;
		string rest1 = csTemp.substr(comma1_idx+1,csTemp.length());
		int comma2_idx = rest1.find_first_of(',');
		string s2 = rest1.substr(0,comma2_idx);
		val = atof(s2.c_str());
		tempnorms[ 3*k + 1 ] = val;
		string rest2 = rest1.substr(comma2_idx+1,rest1.length());
		val = atof(rest2.c_str());
		tempnorms[ 3*k + 2 ] = val;
		k++;
	}

	vin3.close();

	ifstream vin2( fileNameVerts.c_str() );
	double* tempverts = new double[3*iVerts];
	//cout<<"Making object with "<<3*iVerts<<" vertices and normals...\n";
	//double val;
	k = 0;
	while( !vin2.eof() )
	{
		vin2>>line;
		csTemp = line;
		////cout<<csTemp.c_str()<<"\n";
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
	int i;
	for( i = 0; i < (iFaces-1)*3; i++ )
	{
		idx = tempidx[i];

		(*TriVerts)[3*i+0] = tempverts[(idx-1)*3+0];
		(*TriVerts)[3*i+1] = tempverts[(idx-1)*3+1];
		(*TriVerts)[3*i+2] = tempverts[(idx-1)*3+2];

		(*TriNorms)[3*i+0] = tempnorms[(idx-1)*3+0];
		(*TriNorms)[3*i+1] = tempnorms[(idx-1)*3+1];
		(*TriNorms)[3*i+2] = tempnorms[(idx-1)*3+2];

	}

	delete [] tempverts;
	delete [] tempidx;
	delete [] tempnorms;
	iFaces--;  // this is a non-optimal way of handling end of line...
	return iFaces;
}