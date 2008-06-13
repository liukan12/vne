#include "PhysObject.h"
#include "TriObject.h"
#include "QuadObject.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

PhysObject::PhysObject()
{

}

PhysObject::PhysObject(const vector<string> &filenames)
{

}

void PhysObject::ComputeInertia()
{
	this->Inertia = 0.0;

	// diagonal elements
	MyTemp = MassDistribution	 * ( LocVertY * LocVertY + LocVertZ * LocVertZ );
	Inertia[0] = MyTemp.sum();
	MyTemp = MassDistribution	 * ( LocVertX * LocVertX + LocVertZ * LocVertZ );
	Inertia[4] = MyTemp.sum();
	MyTemp = MassDistribution	 * ( LocVertY * LocVertY + LocVertX * LocVertX );
	Inertia[8] = MyTemp.sum();

	// off-diagonal elements
	MyTemp = MassDistribution * LocVertY * LocVertX;
	Inertia[1] = -MyTemp.sum();
	Inertia[3] = Inertia[1];
	MyTemp = MassDistribution * LocVertZ * LocVertX;
	Inertia[2] = -MyTemp.sum();
	Inertia[6] = Inertia[2];
	MyTemp = MassDistribution * LocVertY * LocVertZ;
	Inertia[5] = -MyTemp.sum();
	Inertia[7] = Inertia[5];
}

void PhysObject::ComputeTorqueDistribution()
{
		// torque = r cross F
		TorqueDistributionX = LocVertY*ForceDistributionZ - LocVertZ*ForceDistributionY;
		TorqueDistributionY = -LocVertX*ForceDistributionZ + LocVertZ*ForceDistributionX;
		TorqueDistributionZ = LocVertX*ForceDistributionY - LocVertY*ForceDistributionX;
}

void PhysObject::ComputeRotationMatrix()
{
	double s = Quarternion[0];
	double x = Quarternion[1];
	double y = Quarternion[2];
	double z = Quarternion[3];
	
	Rotation[0] = 1-2*y*y - 2*z*z;
	Rotation[1] = 2*x*y-2*s*z;
	Rotation[2] = 2*x*z+2*s*y;
	Rotation[3] = 2*x*y+2*s*z;
	Rotation[4] = 1-2*x*x-2*z*z;
	Rotation[5] = 2*y*z-2*s*x;
	Rotation[6] = 2*x*z-2*s*y;
	Rotation[7] = 2*y*z+2*s*x;
	Rotation[8] = 1-2*x*x-2*y*y;
}

void PhysObject::ComputeCentroid()
{
	Centroid[0] = CurVertX.sum() / numVerts;
	Centroid[1] = CurVertY.sum() / numVerts;
	Centroid[2] = CurVertZ.sum() / numVerts;

	LocVertX = CurVertX - Centroid[0];
	LocVertY = CurVertY - Centroid[1];
	LocVertZ = CurVertZ - Centroid[2];
}

void PhysObject::AddForceAllVerts( const valarray<double> &Force )
{
	this->ForceDistributionX += Force[0];
	this->ForceDistributionY += Force[1]; 
	this->ForceDistributionZ += Force[2];
}

void PhysObject::AddForceAllVerts( double fx, double fy, double fz )
{
	this->ForceDistributionX += fx;
	this->ForceDistributionY += fy; 
	this->ForceDistributionZ += fz;
}

void PhysObject::AddForceAt(int vertIdx, const valarray<double> &Force )
{
	this->ForceDistributionX[vertIdx] += Force[0];
	this->ForceDistributionY[vertIdx] += Force[1];
	this->ForceDistributionZ[vertIdx] += Force[2];
}

void PhysObject::AddForceAt(int vertIdx, double fx, double fy, double fz )
{
	this->ForceDistributionX[vertIdx] += fx;
	this->ForceDistributionY[vertIdx] += fy;
	this->ForceDistributionZ[vertIdx] += fz;
}

void PhysObject::ComputeNetForce()
{
	NetForce[0] = ForceDistributionX.sum();
	NetForce[1] = ForceDistributionY.sum();
	NetForce[2] = ForceDistributionZ.sum();
}

void PhysObject::ComputeNetTorque()
{
	NetTorque[0] = TorqueDistributionX.sum();
	NetTorque[1] = TorqueDistributionY.sum();
	NetTorque[2] = TorqueDistributionZ.sum();
}

void PhysObject::ComputeRotationalVelocity()
{
	// v = w x r
	RotVelX = AngVel[1]*LocVertZ - AngVel[2]*LocVertY;
	RotVelY = -AngVel[0]*LocVertZ + AngVel[2]*LocVertX;
	RotVelZ = AngVel[0]*LocVertY - AngVel[1]*LocVertX;
}

void PhysObject::TranslateTo(double dx, double dy, double dz)
{
	CurVertX = dx - Centroid[0] + CurVertX;
	CurVertY = dy - Centroid[1] + CurVertY;
	CurVertZ = dz - Centroid[2] + CurVertZ;
	ComputeCentroid();
}

void PhysObject::TranslateBy(double dx, double dy, double dz)
{
	CurVertX += dx;
	CurVertY += dy;
	CurVertZ += dz;
	ComputeCentroid();
}

void PhysObject::AccelerateBy(double dx, double dy, double dz)
{
	Velocity[0] += dx;
	Velocity[1] += dy;
	Velocity[2] += dz;
}

void PhysObject::ComputeRotationMatrix(const valarray<double> &ref, const valarray<double> &vec)
{
	// compute the rotation matrix to take a reference direction to a new direction
	valarray<double> temp(3);
	valarray<double> newdir(3);
	temp = vec*vec;
	newdir = vec / sqrt(temp.sum()); // normalize the vector

	double val = newdir[0]*ref[0] + newdir[1]*ref[1] + newdir[2]*ref[2];
	double theta = acos( val );
	if( theta < 1e-3 ) {
		Rotation = 0.0;
		Rotation[0] = 1.0;
		Rotation[4] = 1.0;
		Rotation[8] = 1.0;
		return;
	}
	valarray<double> uhat(3);
	uhat[0] = ref[1]*newdir[2] - ref[2]*newdir[1];
	uhat[1] = -ref[0]*newdir[2] + ref[2]*newdir[0];
	uhat[2] = ref[0]*newdir[1] - ref[1]*newdir[0];
	temp = uhat*uhat;
	uhat = uhat / sqrt(temp.sum());

	double c1 = cos(theta);
	double s1 = sin(theta);
	double t1 = 1-c1;
	double x = uhat[0];
	double y = uhat[1];
	double z = uhat[2];
	Rotation[0] = x*x*t1+c1;
	Rotation[1] = x*y*t1-z*s1;
	Rotation[2] = x*z*t1+y*s1;
	Rotation[3] = x*y*t1+z*s1;
	Rotation[4] = y*y*t1+c1;
	Rotation[5] = y*z*t1-x*s1;
	Rotation[6] = x*z*t1-y*s1;
	Rotation[7] = y*z*t1+x*s1;
	Rotation[8] = z*z*t1+c1;

}

void PhysObject::GetMinMaxVert()
{
	double minX = 1e3;
	double minY = 1e3;
	double minZ = 1e3;
	double maxX = -1e3;
	double maxY = -1e3;
	double maxZ = -1e3;
	for( int i = 0; i < this->numVerts; i++ )
	{
		if( CurVertX[i] < minX ) {
			minX =  CurVertX[i];
			VertControlPts[0] = i;
		}
		if( CurVertY[i] < minY ) {
			minY =  CurVertY[i];
			VertControlPts[1] = i;
		}
		if( CurVertZ[i] < minZ ) {
			minZ =  CurVertZ[i];
			VertControlPts[2] = i;
		}
		if( CurVertX[i] > maxX ) {
			maxX =  CurVertX[i];
			VertControlPts[3] = i;
		}
		if( CurVertY[i] > maxY ) {
			maxY =  CurVertY[i];
			VertControlPts[4] = i;
		}
		if( CurVertZ[i] > maxZ ) {
			maxZ =  CurVertZ[i];
			VertControlPts[5] = i;
		}
	}
}

void PhysObject::SyncDrawObj()
{
	this->drawObject->UpdateData(CurNormX, CurNormY, CurNormZ,
		CurVertX, CurVertY, CurVertZ, CurIdx );
}

void PhysObject::InitFromFile(const vector<string> &input)
{
	// default settings:
	// input[0] is faces
	// input[1] is norms
	// input[2] is verts
	// string 3 is type of draw object, "tri", or "quad", or other
	if( input.size() != 4 ) {
		cout<<"Error, number of input files for default PhysObject initializer must be 4!\n";
		Error();
	}
	if( input[3] == "tri" ) {
		ProcessTriDataFiles(input);
		this->drawObject = new TriObject(CurNormX, CurNormY, CurNormZ,
								CurVertX, CurVertY, CurVertZ, CurIdx);
	}
	else {
		cout<<"invalid draw object type specified. Try 'tri'.\n";
		Error();
	}

	Inertia = valarray<double>(9);
	Centroid = valarray<double>(3);
	Velocity = valarray<double>(3);
	AngVel = valarray<double>(3);
	Rotation = valarray<double>(9);
	Quarternion = valarray<double>(4);
	MassDistribution = valarray<double>(numVerts);
	ForceDistributionX = valarray<double>(numVerts);
	ForceDistributionY = valarray<double>(numVerts);
	ForceDistributionZ = valarray<double>(numVerts);
	TorqueDistributionX = valarray<double>(numVerts);
	TorqueDistributionY = valarray<double>(numVerts);
	TorqueDistributionZ = valarray<double>(numVerts);
	RotVelX = valarray<double>(numVerts);
	RotVelY = valarray<double>(numVerts);
	RotVelZ = valarray<double>(numVerts);
	NetTorque = valarray<double>(3);
	NetForce = valarray<double>(3);
	LocVertX = valarray<double>(numVerts);
	LocVertY = valarray<double>(numVerts);
	LocVertZ = valarray<double>(numVerts);
	RefVertX = valarray<double>(numVerts);
	RefVertY = valarray<double>(numVerts);
	RefVertZ = valarray<double>(numVerts);
	
	MyTemp = valarray<double>(numVerts);
	MassDistribution = 1.0 / numVerts;
	AngVel[0] = 0.0;
	ComputeRotationMatrix();
	VertControlPts = valarray<int>(6);

	ComputeCentroid();
	CurVertX -= Centroid[0];
	CurVertY -= Centroid[1];
	CurVertZ -= Centroid[2];
	RefVertX = CurVertX;
	RefVertY = CurVertY;
	RefVertZ = CurVertZ;
	ComputeCentroid();
	ComputeInertia();
	Quarternion[0] = 1.0;
}

void PhysObject::ProcessTriDataFiles( const vector<string> filenames )
{
	string fileNameFaces = filenames[0];
	string fileNameNorms = filenames[1];
	string fileNameVerts = filenames[2];
	
	// read a comma separted value file that defines the vertices
	// and indices into vertices for all faces

	char line[512];
	ifstream fin( fileNameFaces.c_str() );
	numFaces = 0;
	
	//cout<<"Start reading faces data..."<<fileNameFaces.c_str()<<"\n";
	// TODO: check for endless loop here if file is not found or is messed up
	while( !fin.eof() )
	{
		fin>>line;
		numFaces++;
		//cout<<numFaces;
		//cout<<line<<"\n";
	}
	fin.close();
	
	//cout<<"Done Reading faces data...";

	CurIdx   = valarray<int>( (numFaces-1)*3 );
	ifstream fin2( fileNameFaces.c_str() );
	
	string csTemp;

	//cout<<"Reading face data...\n";
	//cout<<"Creating object with "<<numFaces<<" triangles\n";
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
		(CurIdx)[ 3*k + 0 ] = idx-1;
		string rest1 = csTemp.substr(comma1_idx+1,csTemp.length());
		int comma2_idx = rest1.find_first_of(',');
		string s2 = rest1.substr(0,comma2_idx);
		idx = atoi(s2.c_str());
		(CurIdx)[ 3*k + 1 ] = idx-1;
		string rest2 = rest1.substr(comma2_idx+1,rest1.length());
		idx = atoi(rest2.c_str());
		(CurIdx)[ 3*k + 2 ] = idx-1;
		k++;
	}

	fin2.close();
	ifstream vin( fileNameVerts.c_str() );
	//cout<<"Counting vertices...\n";
	numVerts = 0;
	k = 0;
	while( !vin.eof() )
	{
		vin>>line;
		numVerts++;
	}
	//numVerts--; // last line is blank
	vin.close();
	//cout<<"Done counting vertices \n";
	//double* tempnorms = new double[3*numVerts];
	CurVertX = valarray<double>(numVerts - 1);
	CurVertY = valarray<double>(numVerts - 1);
	CurVertZ = valarray<double>(numVerts - 1);
	CurNormX = valarray<double>(numVerts - 1);
	CurNormY = valarray<double>(numVerts - 1);
	CurNormZ = valarray<double>(numVerts - 1);
	
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
		CurNormX[ k ] = val;
		string rest1 = csTemp.substr(comma1_idx+1,csTemp.length());
		int comma2_idx = rest1.find_first_of(',');
		string s2 = rest1.substr(0,comma2_idx);
		val = atof(s2.c_str());
		CurNormY[ k ] = val;
		string rest2 = rest1.substr(comma2_idx+1,rest1.length());
		val = atof(rest2.c_str());
		CurNormZ[ k ] = val;
		k++;
	}

	vin3.close();

	ifstream vin2( fileNameVerts.c_str() );
	//double* tempverts = new double[3*numVerts];
	//cout<<"Making object with "<<3*numVerts<<" vertices and normals...\n";
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
		CurVertX[ k ] = val;
		string rest1 = csTemp.substr(comma1_idx+1,csTemp.length());
		int comma2_idx = rest1.find_first_of(',');
		string s2 = rest1.substr(0,comma2_idx);
		val = atof(s2.c_str());
		CurVertY[ k ] = val;
		string rest2 = rest1.substr(comma2_idx+1,rest1.length());
		val = atof(rest2.c_str());
		CurVertZ[ k ] = val;
		k++;
	}
	vin2.close();
	
	/*int i;
	for( i = 0; i < (*CurVert).size(); i++ )
		cout<< (*CurVert)[i] <<", ";*/
	
	numFaces--;
	numVerts--;

}

void PhysObject::UpdateTotals()
{
	this->massTotal = MassDistribution.sum();
	this->ComputeCentroid();
	this->ComputeNetForce();
	this->ComputeTorqueDistribution();
	this->ComputeNetTorque();
	this->ComputeRotationalVelocity();
	this->ComputeInertia();
}

void PhysObject::SetVelocity( double vx, double vy, double vz )
{
	Velocity[0] = vx;
	Velocity[1] = vy;
	Velocity[2] = vz;
}

void PhysObject::UpdateRotation()
{
	this->CurVertX = Rotation[0] * RefVertX + Rotation[1] * RefVertY + Rotation[2] * RefVertZ + Centroid[0];
	this->CurVertY = Rotation[3] * RefVertX + Rotation[4] * RefVertY + Rotation[5] * RefVertZ + Centroid[1];
	this->CurVertZ = Rotation[6] * RefVertX + Rotation[7] * RefVertY + Rotation[8] * RefVertZ + Centroid[2];
}

void PhysObject::UpdateAngVel(double dt)
{
	valarray<double> temp3B(3);
	valarray<double> temp3(3);
	valarray<double> tempq(4);
	// step 2: dw/dt = I \ NetTorque(t)
	temp3B = dt * NetTorque;  // RHS
	temp3 = temp3B;  // initial guess
	for( int i = 0; i < 5; i++ )
	{ // cross fingers and hope 5 Gauss-Seidel iterations work :-)
		temp3[0] = ( temp3B[0] - Inertia[1]*temp3[1] - Inertia[2]*temp3[2] ) / Inertia[0];
		temp3[1] = ( temp3B[1] - Inertia[3]*temp3[0] - Inertia[5]*temp3[2] ) / Inertia[4];
		temp3[2] = ( temp3B[2] - Inertia[6]*temp3[0] - Inertia[7]*temp3[1] ) / Inertia[8];
	}
	this->AngVel = AngVel + temp3;
	
	// step 3: find the new q
	// dq/dt = (1/2) * w X q   , X being quarternion multiplication operation
	// [0,w] X [q]
	tempq[0]	= 0*Quarternion[0] - (AngVel[0]*Quarternion[1] + AngVel[1]*Quarternion[2] + AngVel[2]*Quarternion[3] );
	tempq[1]	= AngVel[1]*Quarternion[3] - AngVel[2]*Quarternion[2];
	tempq[2]	= -AngVel[0]*Quarternion[3] + AngVel[2]*Quarternion[1];
	tempq[3]	= AngVel[0]*Quarternion[2] - AngVel[1]*Quarternion[1];

	tempq[1]	+= 0*Quarternion[1] + Quarternion[0]*AngVel[0];
	tempq[2]	+= 0*Quarternion[2] + Quarternion[0]*AngVel[1];
	tempq[3]	+= 0*Quarternion[3] + Quarternion[0]*AngVel[2];
	
	this->Quarternion = Quarternion + dt * 0.5 * tempq;
	tempq = Quarternion*Quarternion;
	Quarternion = Quarternion / sqrt(tempq.sum());
}

void PhysObject::EulerTimeStep(double dt)
{
	// update position, velocity, angular velocity
	CurVertX = CurVertX + dt*Velocity[0] + 0.5*dt*dt*NetForce[0]/massTotal;
	CurVertY = CurVertY + dt*Velocity[1] + 0.5*dt*dt*NetForce[1]/massTotal;
	CurVertZ = CurVertZ + dt*Velocity[2] + 0.5*dt*dt*NetForce[2]/massTotal;
	Velocity += dt * NetForce / massTotal;

	this->ComputeCentroid();
	this->UpdateAngVel(dt);
	this->ComputeRotationMatrix();
	this->UpdateRotation();
	

	ClearForce();
}

void PhysObject::ClearForce()
{
	ForceDistributionX = 0.0;
	ForceDistributionY = 0.0;
	ForceDistributionZ = 0.0;
}