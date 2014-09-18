#include "../src/optionBarrierLow.h"
#include "../src/optionBasket.h"
#include <iostream>

TEST (OptionBarrierLowTest, Payoff) {
  //PnlVect* p = pnl_vect_new();
  //OptionBarrierLow* o = new OptionBarrierLow(0, 0, 0, 0, p, p);

  double T_ = 3.0;
  int timeSteps_ = 1;
  int size_ = 3;
  double strike_ = 7.0;
  PnlVect* coeffPayoff = pnl_vect_create_from_list (size_, 2.0, 2.0, 2.0);
  const PnlMat* path =  pnl_mat_create_from_list (T_, size_, 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0);
   
  OptionBasket* ob = new OptionBasket(T_,timeSteps_,size_,strike_,coeffPayoff);
  double payoff = ob->payoff(path);
  
  std::cout<<payoff;
    
}

