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

int ReadMeshData( valarray<int> &TriIdx, valarray<double> &TriVertsX, valarray<double> &TriVertsY, valarray<double> &TriVertsZ,
				 valarray<double> &TriNormsX, valarray<double> &TriNormsY, valarray<double> &TriNormsZ, 
				 string fileNameFaces, string fileNameVerts, string fileNameNorms )
{
	// read a comma separted value file that defines the vertices
	// and indices into vertices for all faces

	char line[128];
	ifstream fin( fileNameFaces.c_str() );
	int iFaces = 0;
	
	//cout<<"Start reading faces data..."<<fileNameFaces.c_str()<<"\n";
	
	while( !fin.eof() )
	{
		fin>>line;
		iFaces++;
		//cout<<iFaces;
		//cout<<line<<"\n";
	}
	fin.close();
	
	//cout<<"Done Reading faces data...";

	TriIdx   = valarray<int>( (iFaces-1)*3 );
	

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
		if( csTemp.length() < 2 )
			break;
		////cout<<csTemp.c_str()<<"\n";
		int comma1_idx = csTemp.find_first_of(',');
		string s1 = csTemp.substr(0,comma1_idx);
		idx = atoi(s1.c_str());
		(TriIdx)[ 3*k + 0 ] = idx-1;
		string rest1 = csTemp.substr(comma1_idx+1,csTemp.length());
		int comma2_idx = rest1.find_first_of(',');
		string s2 = rest1.substr(0,comma2_idx);
		idx = atoi(s2.c_str());
		(TriIdx)[ 3*k + 1 ] = idx-1;
		string rest2 = rest1.substr(comma2_idx+1,rest1.length());
		idx = atoi(rest2.c_str());
		(TriIdx)[ 3*k + 2 ] = idx-1;
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
	//double* tempnorms = new double[3*iVerts];
	TriVertsX = valarray<double>(iVerts - 1);
	TriVertsY = valarray<double>(iVerts - 1);
	TriVertsZ = valarray<double>(iVerts - 1);
	TriNormsX = valarray<double>(iVerts - 1);
	TriNormsY = valarray<double>(iVerts - 1);
	TriNormsZ = valarray<double>(iVerts - 1);
	
	ifstream vin3( fileNameNorms.c_str() );
	//cout<<"Reading normals from file "<<fileNameNorms.c_str()<<"\n";
	double val;
	k = 0;
	while( !vin3.eof() )
	{
		vin3>>line;
		csTemp = line;
		if( csTemp.length() < 2 )
			break;
		//cout<<csTemp.c_str()<<"\n";
		int comma1_idx = csTemp.find_first_of(',');
		string s1 = csTemp.substr(0,comma1_idx);
		val = atof(s1.c_str());
		TriNormsX[ k ] = val;
		string rest1 = csTemp.substr(comma1_idx+1,csTemp.length());
		int comma2_idx = rest1.find_first_of(',');
		string s2 = rest1.substr(0,comma2_idx);
		val = atof(s2.c_str());
		TriNormsY[ k ] = val;
		string rest2 = rest1.substr(comma2_idx+1,rest1.length());
		val = atof(rest2.c_str());
		TriNormsZ[ k ] = val;
		k++;
	}

	vin3.close();

	ifstream vin2( fileNameVerts.c_str() );
	//double* tempverts = new double[3*iVerts];
	//cout<<"Making object with "<<3*iVerts<<" vertices and normals...\n";
	//double val;
	k = 0;
	while( !vin2.eof() )
	{
		vin2>>line;
		csTemp = line;
		if( csTemp.length() < 2 )
			break;
		////cout<<csTemp.c_str()<<"\n";
		int comma1_idx = csTemp.find_first_of(',');
		string s1 = csTemp.substr(0,comma1_idx);
		val = atof(s1.c_str());
		TriVertsX[ k ] = val;
		string rest1 = csTemp.substr(comma1_idx+1,csTemp.length());
		int comma2_idx = rest1.find_first_of(',');
		string s2 = rest1.substr(0,comma2_idx);
		val = atof(s2.c_str());
		TriVertsY[ k ] = val;
		string rest2 = rest1.substr(comma2_idx+1,rest1.length());
		val = atof(rest2.c_str());
		TriVertsZ[ k ] = val;
		k++;
	}
	vin2.close();
	
	/*int i;
	for( i = 0; i < (*TriVerts).size(); i++ )
		cout<< (*TriVerts)[i] <<", ";*/
	
	iFaces--;  // this is a non-optimal way of handling end of line...
	return iFaces;
}