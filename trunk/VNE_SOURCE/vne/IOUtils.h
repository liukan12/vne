#ifndef IOUTILS
#define IOUTILS

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

int ReadMeshData( CMatrix** TriVerts, CMatrix** Normals, string fileNameFaces, string fileNameVerts, string fileNameNorms );



#endif