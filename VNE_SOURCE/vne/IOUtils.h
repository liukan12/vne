#ifndef IOUTILS
#define IOUTILS

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <valarray>
#include <numeric>

using namespace std;

int ReadMeshData( valarray<int> &TriIdx, valarray<double> &TriVertsX, valarray<double> &TriVertsY, valarray<double> &TriVertsZ,
				 valarray<double> &TriNormsX, valarray<double> &TriNormsY, valarray<double> &TriNormsZ, 
				 string fileNameFaces, string fileNameVerts, string fileNameNorms );



#endif