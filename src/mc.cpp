#include "mc.h"
#include "bs.h"
#include <cstring>
#include <math.h>

using namespace std;


MonteCarlo::MonteCarlo(Param* P){
  int option_size, timestep_number;
  PnlVect *spot;
  PnlVect *sigma;
  PnlVect *trend;
  double rho, r, maturity;
  P->extract("option size", option_size);
  P->extract("spot", spot, option_size);
  P->extract("volatility", sigma, option_size);
  P->extract("interest rate", r);
  P->extract("correlation", rho);
  P->extract("trend",trend,option_size);
  this->mod_ = new BS(spot, sigma, rho, r, option_size, trend);
  this->opt_ = MonteCarlo::createOption(P);
  P->extract("sample number", this->samples_);

  this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed (this->rng, 0);

  P->extract("maturity", maturity);
  P->extract("timestep number", timestep_number);
  this->h_ = 1/((double) timestep_number);
}

MonteCarlo::~MonteCarlo(){
  delete (this->mod_)->spot_;
  delete (this->mod_)->sigma_;
  delete this->mod_;
  pnl_rng_free(&(this->rng));
  delete this->opt_;
}

/**
 * Cette méthode crée la bonne instance d'option0
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

  //Calcul du payOff   
  double payOffOption=0;
  double mean_payOffSquare=0;
  double tmp;
  
  for(int m=1; m<=this->samples_; m++){
    mod_->asset(path, opt_->T_, opt_->TimeSteps_, this->rng);
    tmp = opt_->payoff(path);
    payOffOption += tmp;
    mean_payOffSquare += tmp*tmp;
  }
  
  payOffOption  = payOffOption/this->samples_;
  mean_payOffSquare = mean_payOffSquare/this->samples_;

  //Calcul du prix de l'option en t=0
  prix = coeffActu * payOffOption;

  //Free path
  pnl_mat_free(&path);

  //Calcul de la largeur de l'intervalle de confinace
  double cst = exp(- 2 * (mod_->r_ * opt_->T_));
  
  double varEstimator = cst * (mean_payOffSquare - (payOffOption*payOffOption));
  
  //Print estimator variance on screen : To be remove ?
  cout<<"Var Estimator: "<<varEstimator<<endl;
  
  ic = (prix + 1.96*sqrt(varEstimator)/sqrt(this->samples_)) - (prix - 1.96*sqrt(varEstimator)/sqrt(this->samples_));
  
}


/**
 * Calcul le prix de l'option en t>0 
 */
void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic){
  
  double coeffActu = exp(- (mod_->r_ * (opt_->T_ - t)) );
  
  //Matrix of assets
  PnlMat* path;
  path= pnl_mat_create(opt_->TimeSteps_+1,(this->mod_)->size_);
  //Calcul du payOff   
  double payOffOption=0;
  double mean_payOffSquare=0;
  double tmp;
  for(int m=1; m<=this->samples_; m++){
    mod_->asset(path, t, opt_->TimeSteps_, opt_->T_, this->rng, past);
    tmp = opt_->payoff(path);
    payOffOption += tmp;
    mean_payOffSquare += tmp*tmp;
  }
  pnl_mat_print(path);
  payOffOption  = payOffOption/this->samples_;
  mean_payOffSquare = mean_payOffSquare/this->samples_;
  
  //calcul du prix de l'option en t>0
  prix = coeffActu * payOffOption;

  //Free path
  pnl_mat_free(&path);

  //Calcul de la largeur de l'intervalle de confinace
  double cst = exp(- 2 * (mod_->r_ * (opt_->T_ - t) ) );
  
  double varEstimator = cst * (mean_payOffSquare - (payOffOption*payOffOption));
  
  //Print estimator variance on screen : To be remove ?
  cout<<"Var Estimator: "<<varEstimator<<endl;
  
  ic = (prix + 1.96*sqrt(varEstimator)/sqrt(this->samples_)) - (prix - 1.96*sqrt(varEstimator)/sqrt(this->samples_));
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic){

  int nbAsset = this->opt_->size_;

  for (int i = 0; i < nbAsset; ++i)
  {
    double sum = 0;
    PnlMat* path = pnl_mat_create(this->opt_->T_+1, nbAsset);
    for (int j = 0; j < this->samples_; ++j)
    {
      this->mod_->asset(path, t, this->opt_->TimeSteps_, this->opt_->T_, this->rng, past);
      PnlMat* path_shift_up = pnl_mat_create(this->opt_->T_+1, nbAsset);
      PnlMat* path_shift_down = pnl_mat_create(this->opt_->T_+1, nbAsset);
      this->mod_->shift_asset(path_shift_up, path, i, this->h_, t, this->opt_->TimeSteps_);
      this->mod_->shift_asset(path_shift_down, path, i, -this->h_, t, this->opt_->TimeSteps_);
      pnl_mat_eq(path_shift_up, path_shift_down);

      // cout << "\n=\n=\n=Path shift up \n\n\n";
      // pnl_mat_print(path_shift_up);
      // cout << "\n=\n=\n=Path shift down \n\n\n";
      // pnl_mat_print(path_shift_down);
      
      sum += this->opt_->payoff(path_shift_up) - this->opt_->payoff(path_shift_down);
      // cout << "Sum = " << sum << endl;
    }
    LET(delta, i) = sum * exp(this->mod_->r_ * (this->opt_->T_ - t) / (2 * this->samples_ * MGET(past, past->m-1, i) * this->h_));
  }
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic){

  int nbAsset = this->opt_->size_;

  for (int i = 0; i < nbAsset; ++i)
  {
    double sum = 0;
    PnlMat* path = pnl_mat_create(this->opt_->T_+1, nbAsset);
    for (int j = 0; j < this->samples_; ++j)
    {
      this->mod_->asset(path, t, this->opt_->TimeSteps_, this->opt_->T_, this->rng, past);
      PnlMat* path_shift_up = pnl_mat_create(this->opt_->T_+1, nbAsset);
      PnlMat* path_shift_down = pnl_mat_create(this->opt_->T_+1, nbAsset);
      this->mod_->shift_asset(path_shift_up, path, i, this->h_, t, this->opt_->TimeSteps_);
      this->mod_->shift_asset(path_shift_down, path, i, -this->h_, t, this->opt_->TimeSteps_);
      pnl_mat_eq(path_shift_up, path_shift_down);

      // cout << "\n=\n=\n=Path shift up \n\n\n";
      // pnl_mat_print(path_shift_up);
      // cout << "\n=\n=\n=Path shift down \n\n\n";
      // pnl_mat_print(path_shift_down);
      
      sum += this->opt_->payoff(path_shift_up) - this->opt_->payoff(path_shift_down);
      // cout << "Sum = " << sum << endl;
    }
    LET(delta, i) = sum * exp(this->mod_->r_ * (this->opt_->T_ - t) / (2 * this->samples_ * MGET(past, past->m-1, i) * this->h_));
  }
}
