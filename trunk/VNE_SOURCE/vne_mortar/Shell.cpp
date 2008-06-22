#include "Shell.h"
#include "Globals.h"

Shell::Shell()
{
	 
}

Shell::Shell( ObjParams params )
{
	this->InitFromFile( params.input );
	this->Velocity = params.InitVelocity;
	this->TranslateTo( params.InitPosition[0], params.InitPosition[1], params.InitPosition[2] );
	this->bIsExploding = false;
	this->UpdateTotals();
	double Ek = 0.5*this->massTotal * ((Velocity*Velocity).sum());
	cout<<"Shell launched with Kinetic Energy "<<Ek<<"\n";
}

void Shell::UpdateSelf()
{
	valarray<double> gravity(3);
	gravity[1] = GRAV/numVerts;

	this->AddForceAllVerts(gravity);
	this->ProcessCollisions();
	this->UpdateTotals();
	this->EulerTimeStep( TIMESTEP );
	this->SyncDrawObj();
	this->GetMinMaxVert();

	if( Velocity[1] < 0 && CurVertY[this->VertControlPts[1]] <= 0 ) {
		Velocity = 0.0;
		this->TranslateBy( 0.0, 0.0 - CurVertY[this->VertControlPts[1]],0.0 );
		 bIsExploding = true;
	}

#ifdef _DEBUG // verify that energy conservation is happening; Etotal better be constant
	double Ek = massTotal * 0.5 * (Velocity*Velocity).sum();
	double Ep = massTotal * (-GRAV) * Centroid[1];
	double Etotal = Ek + Ep;
#endif
	//cout<<"Shell Total Energy: "<<Etotal<<"\n";
	int breakhere = 1;
		
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