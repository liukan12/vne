//
//int VNEObject::TranslateTo(double dx, double dy, double dz)
//{ // warning: this probably breaks now except at object initialization
//	int i;
//	for( i = 0; i < this->numVerts; i++ )
//	{
//		this->CurTriVertX[i] = dx - Centroid[0] + CurTriVertX[i];
//		this->CurTriVertY[i] = dy - Centroid[1] + CurTriVertY[i];
//		this->CurTriVertZ[i] = dz - Centroid[2] + CurTriVertZ[i];
//	}
//
//	ComputeCentroid();
//	return 0;
//}
//
//int VNEObject::TranslateBy(double dx, double dy, double dz)
//{
//	int i;
//	for( i = 0 ; i < this->numVerts ; i++ )
//	{
//		CurTriVertX[i] += dx;
//		CurTriVertY[i] += dy;
//		CurTriVertZ[i] += dz;
//	}
//
//	ComputeCentroid();
//	return 0;
//	
//}