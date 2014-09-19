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
	

	
	//Discretization step
	double h= T / N;
	PnlVect *vectorGaussian;
	vectorGaussian= pnl_vect_create(this->size_);
	//Start by testing if t if a discretization time
	if(t % h == 0){
		pnl_mat_clone(path,past);
		//Loop over time: t+h to T
		for(double i= t; d<= T ; d=d+h){
			
			pnl_vect_rng_normal(vectorGaussian,this->size,rng);
			//Loop on assets
			for(int j= 0; j < this->size_ ;j++){
				//Get the currentPrice
				double computedPrice;
				double currentPrice= pnl_sp_mat_get(path,i,j);
				//Compute the new and set it
				computedPrice= computeIteration(currentPrice,h,j,vectorGaussian);
				pnl_sp_mat_set(path,i+h,j,computedPrice);
			}
		}
	}else{

	}
	pnl_vect_free(&vectorGaussian);


	

}

void BS::computeIteration(double computedPrice, double currentPrice, double h, int assetIndex, PnlVect* vectorGaussian){
	//Simulate the gaussian vector.
	PnlVect *rowChol;
	rowChol= pnl_vect_create(this->size_);
	
	
	pnl_vect_wrap_mat_row(this->chol,assetIndex)

	


	


}

void BS::asset(PnlMat *path, double T, int N, PnlRng *rng){

	//For each time t between 0 and T.
	for(int t=0;t<=T;t= T/N){
		
		PnlVect *vectorGaussian;
		vectorGaussian= pnl_vect_create(this->size_);
		pnl_vect_rng_normal(vectorGaussian,this->size_,rng);
		//For each assets 
		for(int d=0; d<this->size_; d++){

			chol;

			pnl_vect_mult_vect_term(,vectorGaussian);


		}
	}

}







