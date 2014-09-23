#include <iostream>
#include <cstdlib>
#include <cstring>
#include <math.h>  
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
	
	//TODO: Analyze the error message
	int exitChol= pnl_mat_chol(covMatrix);
	cout<<"exitChol "<<exitChol<<endl;
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
	if(fmod(t,h)>= 0 && 0.005>=fmod(t,h)){
		pnl_mat_set_subblock(path,past,0,0);
		int currentIndex= past->m;
		//Loop over time: t+h to T
		for(double i= t; i< T ; i=i+h){	
			pnl_vect_rng_normal(vectorGaussian,this->size_,rng);
			//Loop on assets
			for(int j= 0; j < this->size_ ;j++){
				//Get the currentPrice
				double computedPrice;
				double currentPrice= pnl_mat_get(path,currentIndex-1,j);
				//Compute the new and set it
				computedPrice= computeIteration(currentPrice,h,j,vectorGaussian);
				//pnl_mat_print(path);
				pnl_mat_set(path,currentIndex,j,computedPrice);
							
			}

			currentIndex++;
		}
	}else{
		//Copy past in path except for the last
		pnl_mat_set_subblock(path,past,0,0);
		//Get the last row of past
		PnlVect prices;
		prices= pnl_vect_wrap_mat_row(past,past->m-1);
		int currentIndex= past->m;
		double currentTime= (currentIndex-1)*h;
		//Loop over time: currentTime to T
		for(double i= currentTime; i< T+h ; i=i+h){
			pnl_vect_rng_normal(vectorGaussian,this->size_,rng);
			//cout<<"curr "<<currentIndex<<endl;
			//Loop on assets
			for(int j= 0; j < this->size_ ;j++){
				//Get the currentPrice
				double computedPrice;
				if(currentIndex != past->m ){
					double currentPrice= pnl_mat_get(path,currentIndex-2,j);
					//Compute the new and set it
					computedPrice= computeIteration(currentPrice,h,j,vectorGaussian);
				}else{
					double currentPrice= GET(&prices,j);
					//Compute the new and set it with a different step
	
					computedPrice= computeIteration(currentPrice,t-currentTime,j,vectorGaussian);
				}
				pnl_mat_set(path,currentIndex-1,j,computedPrice);
			}
			
			currentIndex++;
		}

	}
	pnl_vect_free(&vectorGaussian);



}


double BS::computeIteration(double currentPrice, double h, int assetIndex, PnlVect* vectorGaussian){
	//Compute the scalar product
	PnlVect rowChol;
	rowChol= pnl_vect_wrap_mat_row(this->chol,assetIndex);
	double scalarResult= pnl_vect_scalar_prod(&rowChol, vectorGaussian);
	double sigma= pnl_vect_get(this->sigma_,assetIndex); 
	//Compute the exponential argument
	double expArg= sqrt(h)*scalarResult*sigma + h*(this->r_ - (sigma*sigma/2));
	
	return currentPrice*exp(expArg);

}



void BS::asset(PnlMat *path, double T, int N, PnlRng *rng){


	//For each time t between 0 and T.
	assert(N!=0);
	//Initialize the first path row with the spot prices
	for(int j=0; j<this->size_; j++){
      MLET(path,0,j)= GET(this->spot_,j);
  	}
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












