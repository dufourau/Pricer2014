#include <iostream>

using namespace std;

#include "optionPerformance.h"

/**
 * Constructeur de la classe OptionPerformance
 */
OptionPerformance::OptionPerformance(double T_, int timeSteps_, int size_, PnlVect* payoffCoeff): Option(T_, timeSteps_, size_)
{
	this->payoffCoeff_ = payoffCoeff;
}

/**
 *	Destructeur de la classe OptionPerformance
 */
 OptionPerformance::~OptionPerformance(){
 	pnl_vect_free(&(this->payoffCoeff_));
 }

/**
 * Implémentation de la méthode de payoff
 */
 double OptionPerformance::payoff(const PnlMat *path){

 	double ratio = 0.0;
 	double num = 0.0; 
 	double den = 0.0;

 	PnlVect * assetAtToday = pnl_vect_new();

 	//Sum coeff ratio
 	for(int i = 1 ; i < path->m ; i++){

 		//On extrait la ligne a t=t_i
  		pnl_mat_get_row(assetAtToday, path, i);
  		//On extract la ligne a t=t_i-1 
  		pnl_mat_get_row(assetAtToday, path, i-1); 
  
 		//Compute numerator and denom
 		for(int d = 1 ; d < path->n ; d++){
 			
 			num += GET(assetAtToday,d)*GET(payoffCoeff_,i);
 			den +=  GET(assetAtToday,d)*GET(payoffCoeff_,i-1);
 			
 		}
 		//Compute of ratio
 		ratio += num/den - 1; 
 	}

 	// Divide by N
 	ratio = ratio / path->m;
	
	//Refresh ratio
 	if(ratio < 0){
 		ratio = 0;
 	}
 	
 	 pnl_vect_free(&assetAtToday);


 	//Compute min
 	if(ratio > 0.1){
 		return 1.1;
 	}else{
 		return 1 + ratio;
 	}
 	
 	
 }
