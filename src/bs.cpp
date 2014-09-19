#include <iostream>
#include <cstdlib>
#include <cstring>
#include "assert.h"

using namespace std;

#include "bs.h"


//Blacḱ&Scholes Constructor
BS::BS(PnlVect *spot_, PnlVect *sigma_,double rho_,double r_,int size_)
{
	this->spot_= spot_;
	this->sigma_= sigma_;
	this->rho_= rho_;
	this->r_= r_;
	this->size_= size_;
	//Compute a the cholesky factorization 
	PnlMat *chol;
	chol= pnl_mat_create(size_,size_);
	computeCholesky(chol,rho_);
	this->chol= chol;
}
	
BS::~BS()
{
	//Free the cholesky matrix
	pnl_mat_free(&chol);
}

void BS::computeCholesky(PnlMat *chol,double rho_)
{
	//Intial correlation matrix
	PnlMat *covMatrix;
	double size_= this->size_;
	//Fill the matrix with the correlation factor.
	covMatrix= pnl_mat_create_from_scalar(size_,size_, rho_);
	//Set the diagonal to 1.	
	pnl_mat_set_diag(covMatrix,1,0);
	cout<<"covMatrix"<<endl;
	pnl_mat_print(covMatrix);
	
	//TODO: Analyze the error message
	int exitChol= pnl_mat_chol(covMatrix);
	cout<<"exitChol "<<exitChol<<endl;
	cout<<"cholesky"<<endl;
	pnl_mat_print(covMatrix);
	//Clone the result
	pnl_mat_clone (chol, covMatrix);
	//Free the temp matrix
	pnl_mat_free(&covMatrix);


}


void BS::asset(PnlMat *path, double t, int N, double T, PnlRng *rng, const PnlMat *past)
{
	
}


double BS::computeIteration(double currentPrice, double h, int assetIndex, PnlVect* vectorGaussian)
{
	//Simulate the gaussian vector.
	PnlVect *rowChol;
	rowChol= pnl_vect_create(this->size_);
	pnl_vect_wrap_mat_row(this->chol,assetIndex);
}

void BS::asset(PnlMat *path, double T, int N, PnlRng *rng)
{

	//For each time t between 0 and T.
	assert(N!=0);
	assert(((int)T)%N==0);
	PnlVect *vectorGaussian;
	vectorGaussian= pnl_vect_create(this->size_);

	for(int i=1;i<N+1;i++){
		pnl_vect_rng_normal(vectorGaussian,this->size_,rng);
		//For each assets 
		for(int j=0; j<this->size_; j++){
			MLET(path,i,j)=this->computeIteration(MGET(path,i-1,j),T/N,j,vectorGaussian);
		}
	}
	pnl_vect_free(&vectorGaussian);
}







