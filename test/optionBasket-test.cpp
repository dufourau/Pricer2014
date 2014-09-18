#include "gtest/gtest.h"
#include "../src/optionBarrierLow.h"
#include "../src/optionBasket.h"
#include <iostream>

TEST (OptionBasket, Payoff) {
  double T_ = 3.0;
  int timeSteps_ = 1;
  int size_ = 3;
  double strike_ = 5.0;
  PnlVect* coeffPayoff = pnl_vect_create_from_list (size_, 2.0, 2.0, 2.0);
  const PnlMat* path =  pnl_mat_create_from_list (T_, size_, 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0);
   
  OptionBasket* ob = new OptionBasket(T_,timeSteps_,size_,strike_,coeffPayoff);
  double payoff = ob->payoff(path);
  
  ASSERT_EQ(payoff,1.0);
}
