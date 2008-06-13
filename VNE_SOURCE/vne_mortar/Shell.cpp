#include "Shell.h"

Shell::Shell()
{
	
}

Shell::Shell( ObjParams params )
{
	this->InitFromFile( params.input );
	this->Velocity = params.InitVelocity;
	this->TranslateTo( params.InitPosition[0], params.InitPosition[1], params.InitPosition[2] );
}

void Shell::UpdateSelf()
{
	valarray<double> gravity(3);
	valarray<double> airres(3);
	airres = -0.001 * Velocity;
	gravity[1] = -9.8 * .01;
	this->AddForceAllVerts(gravity);
	this->AddForceAllVerts(airres);
	this->UpdateTotals();
	this->EulerTimeStep( 0.01 );
	this->SyncDrawObj();
	this->GetMinMaxVert();

	if( Velocity[1] < 0 && CurVertY[this->VertControlPts[1]] <= 0 )
		Explode();
}

Shell::Shell(const Shell &)
{

}

void Shell::GrabCamera()
{

}

void Shell::Explode()
{
	cout<<"Shell Explodes at Location (x,y,z) = "<<Centroid[0]<<","<<Centroid[1]<<","<<Centroid[2]<<"\n";
}