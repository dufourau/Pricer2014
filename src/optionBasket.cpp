#include <iostream>

using namespace std;

#include "optionBasket.h"

OptionBasket::OptionBasket(double T_, int timeSteps_, int size_, double strike_, PnlVect* payoffCoeff_) : Option(T_, timeSteps_, size_){
  this->strike_ = strike_;
  this->payoffCoeff_ = pnl_vect_copy(payoffCoeff_);
}

OptionBasket::~OptionBasket(){
  pnl_vect_free(&(this->payoffCoeff_));
}

double OptionBasket::payoff(const PnlMat *path){
  //double res = 0.0;

  //int i;
  //PnlVect * assetAtMaturity = pnl_vect_create(this->size);
  

  
}
