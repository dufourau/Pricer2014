#include "mc.h"
#include "bs.h"
#include <cstring>
#include <math.h>


MonteCarlo::MonteCarlo(Param* P){
  int option_size, timestep;
  PnlVect *spot;
  PnlVect *sigma;
  double rho, r, maturity;
  P->extract("option size", option_size);
  P->extract("spot", spot, option_size);
  P->extract("volatility", sigma, option_size);
  P->extract("interest rate", r);
  P->extract("correlation", rho);

  this->mod_ = new BS(spot, sigma, rho, r, option_size);
  this->opt_ = MonteCarlo::createOption(P);
  P->extract("sample number", this->samples_);

  this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed (this->rng, 0);

  P->extract("maturity", maturity);
  this->h_ = maturity/((double) timestep);
}

MonteCarlo::~MonteCarlo(){
  delete this->mod_;
}

/**
 * Cette méthode crée la bonne instance d'option
 */
Option* MonteCarlo::createOption(Param *P){
  
  double maturity, strike;
  PnlVect *payoffCoeff, *lowerBarrier, *upperBarrier;
  int time_steps,option_size;

  char* key;
  P->extract("option type", key);
  P->extract("maturity", maturity);
  P->extract("timestep number", time_steps);
  P->extract("option size", option_size);
  
  if(strcmp(key,"basket")==0){
      P->extract("strike", strike);
      P->extract("payoff coefficients", payoffCoeff, option_size);
      Option* op = new OptionBasket(maturity, time_steps, option_size, strike, payoffCoeff);
      return op;
  }
      
  else if(strcmp(key,"asian")==0){
    P->extract("strike", strike);
    Option* op = new OptionAsian(maturity, time_steps, option_size, strike);
    return op;
  }  
  
  else if(strcmp(key,"barrier_l")==0){
    P->extract("strike", strike);
    P->extract("payoff coefficients", payoffCoeff, option_size);
    P->extract("lower barrier", lowerBarrier, option_size);
    Option* op = new OptionBarrierLow(maturity, time_steps, option_size, strike, payoffCoeff,lowerBarrier);
    return op;
  }

  else if(strcmp(key,"barrier_u")==0){
    P->extract("strike", strike);
    P->extract("payoff coefficients", payoffCoeff, option_size);
    P->extract("upper barrier", upperBarrier, option_size);
    Option* op = new OptionBarrierUp(maturity, time_steps, option_size, strike, payoffCoeff,upperBarrier);
    return op;
  }

  else if(strcmp(key,"barrier")==0){
    P->extract("strike", strike);
    P->extract("payoff coefficients", payoffCoeff, option_size);
    P->extract("lower barrier", lowerBarrier, option_size);
    P->extract("upper barrier", upperBarrier, option_size);
    Option* op = new OptionBarrier(maturity, time_steps, option_size, strike, payoffCoeff,lowerBarrier,upperBarrier);
    return op;
  }
  
  else if(strcmp(key,"performance")==0){
    P->extract("payoff coefficients", payoffCoeff, option_size);
    Option* op = new OptionPerformance(maturity, time_steps, option_size, payoffCoeff);
    return op;
  }

  else{
    return NULL;
  }
}


/**
 * Calcul le prix de l'option en t=0 et la largeur de son intervalle de confinace
 */
void MonteCarlo::price(double &prix, double &ic){
  double coeffActu = exp(- (mod_->r_ * opt_->T_) );
  
  //Matrix of assets
  //Initialize with spot
  PnlMat* path;
  path= pnl_mat_create(opt_->TimeSteps_+1,(this->mod_)->size_);
  mod_->asset(path, opt_->T_, opt_->TimeSteps_, this->rng);
  
  //Calcul du payOff   
  double payOffOption = opt_->payoff(path);
  

  //Calcul du prix de l'option en t=0
  prix = coeffActu * payOffOption;


  //Free path
  pnl_mat_free(&path);
  //Calcul de la largeur de l'intervalle de confinace
  double cst = exp(- 2 * (mod_->r_ * opt_->T_));
  
  double varEstimator;
  
  for(int i=0; i<mod_->size_; i++){
    varEstimator += payOffOption*payOffOption;
  }
  
  varEstimator = varEstimator/mod_->size_;
  varEstimator -= payOffOption*payOffOption;
  varEstimator = varEstimator*cst;
  
  ic = (prix + 1.96*sqrt(varEstimator)/sqrt(mod_->size_)) - (prix - 1.96*sqrt(varEstimator)/sqrt(mod_->size_));
  
}


/**
 * Calcul le prix de l'option en t>0 
 */
void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic){
  
  double coeffActu = exp(- (mod_->r_ * (opt_->T_ - t)) );
  
  //Matrix of assets
  PnlMat* path = pnl_mat_new();
  path= pnl_mat_create(opt_->TimeSteps_+1,(this->mod_)->size_);
  mod_->asset(path, t, opt_->TimeSteps_, opt_->T_, this->rng, past);
  
  //calcul du payoff
  double payOffOption = opt_->payoff(path);
  
  //calcul du prix de l'option en t>0
  prix = coeffActu * payOffOption;
}
