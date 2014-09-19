#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

#include "bs.h"


//Blacḱ&Scholes Constructor
BS::BS(PnlVect *spot_, PnlVect *sigma_,double rho_,double r_,int size_){
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
	
BS::~BS(){
	//Free the cholesky matrix
	pnl_mat_free(&chol);
}

void BS::computeCholesky(PnlMat *chol,double rho_){
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

void BS::asset(PnlMat *path, double t, int N, double T, PnlRng *rng, const PnlMat *past){
	

	PnlVect *vectorGaussian;
	vectorGaussian= pnl_vect_create(this->size_);
	pnl_vect_rng_normal(vectorGaussian,10,rng);
	cout<<"Loi normale standard"<<endl;
	pnl_vect_print(vectorGaussian);
	pnl_vect_free(&vectorGaussian);
	

}

void BS::asset(PnlMat *path, double T, int N, PnlRng *rng){
  
  NULL;
}



