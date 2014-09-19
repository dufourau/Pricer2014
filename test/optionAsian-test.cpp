#include "gtest/gtest.h"
#include "../src/optionAsian.h"
#include <iostream>

TEST (OptionAsian1, Payoff) {
  
  double strike=3.0;
  double T=10.0;
  int TimeSteps=5;
  int size=10;
  PnlMat *mat=  pnl_mat_create_from_list(6,1,3.0,3.0,3.0,3.0,3.0,3.0);
  PnlVect* coeffPayoff = pnl_vect_create_from_list (size, 2.0, 2.0, 2.0);
  OptionAsian *O=new OptionAsian(strike,coeffPayoff,T,TimeSteps,size);
  double payoff = O->payoff(mat);
  pnl_mat_free(&mat);
  pnl_vect_free(&coeffPayoff);
  ASSERT_GT(payoff,0.0);
  
}

TEST (OptionAsian2, Payoff) {
  
  double strike=3.0;
  double T=10.0;
  int TimeSteps=5;
  int size=10;
  PnlMat *mat=  pnl_mat_create_from_list(6,1,1.0,1.0,1.0,1.0,1.0,1.0);
  PnlVect* coeffPayoff = pnl_vect_create_from_list (size, 2.0, 2.0, 2.0);
  OptionAsian *O=new OptionAsian(strike,coeffPayoff,T,TimeSteps,size);
  double payoff = O->payoff(mat);
  pnl_mat_free(&mat);
  pnl_vect_free(&coeffPayoff);
  ASSERT_EQ(payoff,0.0);
}
