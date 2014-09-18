#include <iostream>

using namespace std;

#include "optionBasket.h"

/**
 * Constructeur de la classe OptionBasket
 */
OptionBasket::OptionBasket(double T_, int timeSteps_, int size_, double strike_, PnlVect* payoffCoeff_) : Option(T_, timeSteps_, size_){
  this->strike_ = strike_;
  this->payoffCoeff_ = pnl_vect_copy(payoffCoeff_);
}

/**
 * Destructeur de la classe OptionBasket
 */
OptionBasket::~OptionBasket(){
  pnl_vect_free(&(this->payoffCoeff_));
}

/**
 * Implémentation de la méthode de payoff
 */
double OptionBasket::payoff(const PnlMat *path){
  double res = 0.0;

  int i;
  PnlVect * assetAtMaturity = pnl_vect_new();

  //On extrait la ligne 
  pnl_mat_get_row(assetAtMaturity, path, (this->T_)-1); 
  
  if(assetAtMaturity->size == payoffCoeff_->size){
    int size = assetAtMaturity->size;
    for(int i = 0; i<size; i++){
      res += GET(assetAtMaturity,i)*GET(payoffCoeff_,i);
    }

    res -= this->strike_;

    if(res<=0.0){
      return 0.0;
    }
    
  }

  return res;
  
}
