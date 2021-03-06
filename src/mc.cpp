#include "mc.h"
#include "bs.h"
#include <cstring>
#include <math.h>

using namespace std;


MonteCarlo::MonteCarlo(Param* P,int H)
{
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
  this->mod_ = new BS(spot, sigma, rho, r, option_size, trend,H);
  this->opt_ = MonteCarlo::createOption(P);
  this->H_= H;
  P->extract("sample number", this->samples_);

  this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed (this->rng, 0);

  P->extract("maturity", maturity);
  P->extract("timestep number", timestep_number);
  //The market step set as 0.1 
  this->h_ = 0.1;
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
  cout<< "Intervalle de confiance: " << ic << endl;
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
  
  
  ic = (prix + 1.96*sqrt(varEstimator)/sqrt(this->samples_)) - (prix - 1.96*sqrt(varEstimator)/sqrt(this->samples_));
}





void MonteCarlo::freeRiskInvestedPart(PnlVect *V,double T, double &profitLoss){

    PnlMat *simulMarketResult,*tempMarketResult;
    simulMarketResult= pnl_mat_create(this->H_+1,this->mod_->size_);
    //Simulate H+1 values from 0 to T (market values)




    mod_->simul_market(simulMarketResult,T,this->H_,this->rng);

    PnlVect* precDelta, *ecartDelta, *copydelta_i;
    //Current Time of iteration
    double tho=0.0;
    //Compute V0
    double refprice,refic;
    this->price(refprice,refic);
    cout<<"price :"<<refprice<<endl;
    PnlVect* delta,*ic;
    delta= pnl_vect_create(this->mod_->size_);
    ic= pnl_vect_create(this->mod_->size_);
    //Get the first delta
    this->delta(simulMarketResult,tho,delta,ic);
    cout<<"delta "<< tho<<endl;
    pnl_vect_print(delta);
    PnlVect *s;
    s = pnl_vect_create(this->mod_->size_);
    pnl_mat_get_row(s,simulMarketResult,0);
    LET(V,0)=refprice - pnl_vect_scalar_prod(delta,s);
    precDelta= pnl_vect_copy(delta);
  
    
    //We will itirate 
    if(V->size>0){

      for(int i=1; i<V->size-1;i++){

        tho+=T/((double) this->H_);
        //Extract the row from 0 to tho "time"
        tempMarketResult= pnl_mat_create(i,this->mod_->size_);
        pnl_mat_extract_subblock (tempMarketResult, simulMarketResult, 0, i+1, 0, this->mod_->size_);
    
        //pnl_vect_print(delta);
        //pnl_mat_print(tempMarketResult); 
        this->delta(tempMarketResult, tho, delta,ic);
        cout<<"delta "<< tho<<endl;
        pnl_vect_print(delta);
        copydelta_i=pnl_vect_copy(delta);
        pnl_vect_minus_vect(copydelta_i,precDelta);
        pnl_mat_get_row(s,simulMarketResult,i);
        LET(V,i)=GET(V,i-1)*exp(mod_->r_ * T / ((double) this->H_)) - pnl_vect_scalar_prod(copydelta_i,s);
        precDelta= pnl_vect_copy(delta);
        
        pnl_mat_free(&tempMarketResult);
      }
    }
    //Assume we don't rebalance for the last date
    pnl_mat_get_row(s,simulMarketResult,V->size-1);
    LET(V,V->size-1)=GET(V,V->size-2)*exp(mod_->r_ * T / ((double) this->H_));
    profitLoss=GET(V,V->size-1)+pnl_vect_scalar_prod(precDelta,s)-this->opt_->payoff(simulMarketResult);
    pnl_vect_free(&s);
    pnl_vect_free(&delta);
    pnl_vect_free(&ic);
    pnl_vect_free(&precDelta);

    pnl_mat_free(&simulMarketResult);



}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic){
  int nbAsset = this->opt_->size_;
  PnlMat* path_shift_up = pnl_mat_create(this->opt_->TimeSteps_+1, nbAsset);
  PnlMat* path_shift_down = pnl_mat_create(this->opt_->TimeSteps_+1, nbAsset);
  PnlMat* path = pnl_mat_create(this->opt_->TimeSteps_+1, nbAsset);
  PnlVect* sum=pnl_vect_create(nbAsset);
  double tstep=this->opt_->T_/this->opt_->TimeSteps_;
  
  for (int j = 0; j < this->samples_; ++j)
  {
    //cout<<"Index sample: " << j<<endl;
    //Select the right asset method to call  
    if(t==0){
        this->mod_->asset(path, this->opt_->T_, this->opt_->TimeSteps_, this->rng);
    }else{
        
        this->mod_->asset(path, t, this->opt_->TimeSteps_, this->opt_->T_, this->rng, past);

    }

    for (int i = 0; i < nbAsset; ++i)
    {
      
      
      this->mod_->shift_asset(path_shift_up, path, i, this->h_, t, tstep);
 
      this->mod_->shift_asset(path_shift_down,path, i, -this->h_, t, tstep);
      
      LET(sum,i)=GET(sum,i)+this->opt_->payoff(path_shift_up) - this->opt_->payoff(path_shift_down);
      /*
      cout<<"t "<<t<<endl;
      cout<<"path"<<endl;
      pnl_mat_print(path);
      cout<<"path_shift_down"<<endl;
      pnl_mat_print(path_shift_up);
      cout<<"path_shift_up"<<endl;
      pnl_mat_print(path_shift_down);
      cout<<"t!!!!!!!!!!!!!!!!!!!!!!!"<<t<<endl;
      cout<<"payoff up "<< this->opt_->payoff(path_shift_up)<<endl;
      cout<<"payoff up "<< this->opt_->payoff(path_shift_down)<<endl;
      */

    }
  }

  for (int i = 0; i < nbAsset; i++)
  {
    if(t==0){
        LET(delta, i) = GET(sum,i) * exp(-this->mod_->r_ * (this->opt_->T_ - t)) / (2.0 * this->samples_ * MGET(path, 0, i) * this->h_);
    }else{

        LET(delta, i) = GET(sum,i) * exp(-this->mod_->r_ * (this->opt_->T_ - t)) / (2.0 * this->samples_ * MGET(past, past->m-1, i) * this->h_);  
    }
    

  }

  pnl_vect_free(&sum);
  pnl_mat_free(&path);
  pnl_mat_free(&path_shift_up);
  pnl_mat_free(&path_shift_down);
}
 