#include "mc.h"
#include <cstring>

Option* MonteCarlo::createOption(char* key, Param *P){
  
  double maturity, strike;
  PnlVect *payoffCoeff;
  int time_steps,option_size;
  
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
    return NULL;
  }  
  
  else if(strcmp(key,"barrier_l")==0){
    return NULL;
  }

  else if(strcmp(key,"barrier_u")==0){
    return NULL;
  }

  else if(strcmp(key,"barrier")==0){
    return NULL;
  }
  
  else if(strcmp(key,"performance")==0){
    return NULL;
  }

  else{
    return NULL;
  }
}
