#include <iostream>
#include <cstdlib>
#include <cstring>
#include <math.h>  
#include "assert.h"

using namespace std;

#include "bs.h"


//Blacḱ&Scholes Constructor
BS::BS(PnlVect *spot_, PnlVect *sigma_,double rho_,double r_,int size_,PnlVect *trend, int H)
{
	this->spot_= spot_;
	this->H_= H;
	this->sigma_= sigma_;
	this->rho_= rho_;
	this->trend=trend;
	this->r_= r_;
	this->size_= size_;
	//Compute a the cholesky factorization 
	PnlMat *chol;
	chol= pnl_mat_create(size_,size_);
	computeCholesky(chol,rho_);
	this->chol= chol;
}
//Blacḱ&Scholes Destructor		
BS::~BS()
{
	//Free the cholesky matrix
	pnl_mat_free(&chol);
}

void BS::computeCholesky(PnlMat *chol,double rho_)
{
	//Initial correlation matrix
	PnlMat *covMatrix;
	double size_= this->size_;
	//Fill the matrix with the correlation factor.
	covMatrix= pnl_mat_create_from_scalar(size_,size_, rho_);
	//Set the diagonal to 1.	
	pnl_mat_set_diag(covMatrix,1,0);
	int exitChol= pnl_mat_chol(covMatrix);

	if(exitChol != 0 ){
		cout<<"Cholesky failed"<<endl;
		throw std::exception();
	}
	//Clone the result
	pnl_mat_clone (chol, covMatrix);
	//Free the temp matrix
	pnl_mat_free(&covMatrix);
}

void BS::asset(PnlMat *path, double t, int N, double T, PnlRng *rng, const PnlMat *past){
	
	//Discretization step
	//TODO Add case with the minimum step (real and simulated)
	double h= T / N;
	//Compute the error of modulo
	PnlVect *vectorGaussian;
	vectorGaussian= pnl_vect_create(this->size_);
	//Start by testing if t is a discretization time
	if(fmod(t,h)>= 0 && 0.005>=fmod(t,h)){
		pnl_mat_set_subblock(path,past,0,0);
		int currentIndex= past->m;
		//Loop over time: t+h to T
		for(double i= t; i< T ; i=i+h){	
			pnl_vect_rng_normal(vectorGaussian,this->size_,rng);
			//Loop on assets
			for(int j= 0; j < this->size_ ;j++){
				//Get the currentPrice
				double currentPrice= MGET(path,currentIndex-1,j);
				//pnl_mat_print(path);
				MLET(path,currentIndex,j)=computeIteration(currentPrice,h,j,vectorGaussian, false);
							
			}

			currentIndex++;
		}
	}else{
		//Copy past in path (The last row will be overwritten)
		pnl_mat_extract_subblock(path,past,0,past->m-1,0,past->n);
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
					double currentPrice= MGET(path,currentIndex-2,j);
					//Compute the new and set it
					computedPrice= computeIteration(currentPrice,h,j,vectorGaussian,false);
				}else{
					double currentPrice= GET(&prices,j);
					//Compute the new price and set it with a different step
					computedPrice= computeIteration(currentPrice,h-t+currentTime,j,vectorGaussian,false);

				}
				MLET(path,currentIndex-1,j)=computedPrice;
			}
			
			currentIndex++;
		}

	}
	pnl_vect_free(&vectorGaussian);
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
		//Create the gaussian for each simulation
		pnl_vect_rng_normal(vectorGaussian,this->size_,rng);
		//For each assets 
		for(int j=0; j<this->size_; j++){
			MLET(path,i,j)=this->computeIteration(MGET(path,i-1,j),T/N,j,vectorGaussian,false);
		}
	}

	pnl_vect_free(&vectorGaussian);
}

double BS::computeIteration(double currentPrice, double h, int assetIndex, PnlVect* vectorGaussian, bool useTrend){
	//Compute the scalar product
	PnlVect rowChol;
	rowChol= pnl_vect_wrap_mat_row(this->chol,assetIndex);
	double scalarResult= pnl_vect_scalar_prod(&rowChol, vectorGaussian);
	double sigma=GET(this->sigma_,assetIndex); 
	//Compute the exponential argument
	double expArg;
	double mu;
	if (useTrend){
		mu=GET(this->trend,assetIndex);
		expArg= sqrt(h)*scalarResult*sigma + h*(mu - (sigma*sigma/2));
	}else{
		expArg= sqrt(h)*scalarResult*sigma + h*(this->r_ - (sigma*sigma/2));
	}
	return currentPrice*exp(expArg);

}

void BS::shift_asset(PnlMat *shift_path, const PnlMat *path,int d, double h, double t, double timestep){
	//Clone the in matrix into the out matrix
	pnl_mat_clone(shift_path, path);
	//Calculate the index associate to the time t
	int index = (int) t*timestep;
	for (int i = index ; i < path->m; ++i)
	{
		MLET(shift_path, i, d) *= (1+h);
	}
}

void BS::simul_market(PnlMat *path, double T, int H, PnlRng *rng){
	
	assert(H!=0);
	//Initialize the first path row with the spot prices
	for(int j=0; j<this->size_; j++){
      MLET(path,0,j)= GET(this->spot_,j);
  	}
	PnlVect *vectorGaussian;
	vectorGaussian= pnl_vect_create(this->size_);
	for(int i=1;i<H+1;i++){
		pnl_vect_rng_normal(vectorGaussian,this->size_,rng);
		//For each assets 
		for(int j=0; j<this->size_; j++){
			MLET(path,i,j)=this->computeIteration(MGET(path,i-1,j),T/H,j,vectorGaussian,true);
		}
	}
	pnl_vect_free(&vectorGaussian);
}
