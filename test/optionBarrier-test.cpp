#include "gtest/gtest.h"
#include "../src/optionBarrierLow.h"
#include "../src/optionBarrierUp.h"
#include "../src/optionBarrier.h"
#include <iostream>

TEST (OptionBarrierLowTest, PayoffNotNull) {
  double T_ = 3.0;
  int timeSteps_ = 1;
  int size_ = 3;
  double strike_ = 5.0;
  PnlVect* coeffPayoff = pnl_vect_create_from_list (size_, 2.0, 2.0, 2.0);
  const PnlMat* path =  pnl_mat_create_from_list (T_, size_, 2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5);
  const PnlVect* lowerBarrier = pnl_vect_create_from_list(size_, 2.0, 2.0, 2.0);

  OptionBarrierLow* ob = new OptionBarrierLow(T_,timeSteps_,size_,strike_,coeffPayoff, lowerBarrier);
  double payoff = ob->payoff(path);
  
  ASSERT_EQ(payoff,10);
}

TEST (OptionBarrierLowTest, PayoffNull) {
  double T_ = 3.0;
  int timeSteps_ = 1;
  int size_ = 3;
  double strike_ = 5.0;
  PnlVect* coeffPayoff = pnl_vect_create_from_list (size_, 2.0, 2.0, 2.0);
  const PnlMat* path =  pnl_mat_create_from_list (T_, size_, 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0);
  const PnlVect* lowerBarrier = pnl_vect_create_from_list(size_, 2.0, 2.0, 2.0);

  OptionBarrierLow* ob = new OptionBarrierLow(T_,timeSteps_,size_,strike_,coeffPayoff, lowerBarrier);
  double payoff = ob->payoff(path);
  
  ASSERT_EQ(payoff,0);
}

TEST (OptionBarrierUpTest, PayoffNull) {
  double T_ = 3.0;
  int timeSteps_ = 1;
  int size_ = 3;
  double strike_ = 5.0;
  PnlVect* coeffPayoff = pnl_vect_create_from_list (size_, 2.0, 2.0, 2.0);
  const PnlMat* path =  pnl_mat_create_from_list (T_, size_, 2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5);
  const PnlVect* upperBarrier = pnl_vect_create_from_list(size_, 2.0, 2.0, 2.0);

  OptionBarrierUp* ob = new OptionBarrierUp(T_,timeSteps_,size_,strike_,coeffPayoff, upperBarrier);
  double payoff = ob->payoff(path);
  
  ASSERT_EQ(payoff,0);
}

TEST (OptionBarrierUpTest, PayoffNotNull) {
  double T_ = 3.0;
  int timeSteps_ = 1;
  int size_ = 3;
  double strike_ = 5.0;
  PnlVect* coeffPayoff = pnl_vect_create_from_list (size_, 2.0, 2.0, 2.0);
  const PnlMat* path =  pnl_mat_create_from_list (T_, size_, 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0);
  const PnlVect* upperBarrier = pnl_vect_create_from_list(size_, 2.0, 2.0, 2.0);

  OptionBarrierUp* ob = new OptionBarrierUp(T_,timeSteps_,size_,strike_,coeffPayoff, upperBarrier);
  double payoff = ob->payoff(path);
  
  ASSERT_EQ(payoff,1);
}

TEST (OptionBarrierTest, PayoffNotNull) {
  double T_ = 3.0;
  int timeSteps_ = 1;
  int size_ = 3;
  double strike_ = 5.0;
  PnlVect* coeffPayoff = pnl_vect_create_from_list (size_, 2.0, 2.0, 2.0);
  const PnlMat* path =  pnl_mat_create_from_list (T_, size_, 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0);
  const PnlVect* upperBarrier = pnl_vect_create_from_list(size_, 2.0, 2.0, 2.0);
  const PnlVect* lowerBarrier = pnl_vect_create_from_list(size_, 0.0, 0.0, 0.0);

  OptionBarrier* ob = new OptionBarrier(T_,timeSteps_,size_,strike_,coeffPayoff, lowerBarrier, upperBarrier);
  double payoff = ob->payoff(path);
  
  ASSERT_EQ(payoff,1);
}

TEST (OptionBarrierTest, PayoffNull) {
  double T_ = 3.0;
  int timeSteps_ = 1;
  int size_ = 3;
  double strike_ = 5.0;
  PnlVect* coeffPayoff = pnl_vect_create_from_list (size_, 2.0, 2.0, 2.0);
  const PnlMat* path =  pnl_mat_create_from_list (T_, size_, 2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5);
  const PnlVect* lowerBarrier = pnl_vect_create_from_list(size_, 2.0, 2.0, 2.0);
  const PnlVect* upperBarrier = pnl_vect_create_from_list(size_, 0.0, 0.0, 0.0);

  OptionBarrier* ob = new OptionBarrier(T_,timeSteps_,size_,strike_,coeffPayoff, lowerBarrier, upperBarrier);
  double payoff = ob->payoff(path);
  
  ASSERT_EQ(payoff,0);
}
