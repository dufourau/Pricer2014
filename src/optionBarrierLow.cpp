#include "optionBarrierLow.h"
#include <iostream>

using namespace std;

OptionBarrierLow::OptionBarrierLow(double T_, int timeSteps_, int size_, double strike_, PnlVect* payoffCoeff_, PnlVect* lowerBarrier_) : Option(T_, timeSteps_, size_)
{
  this->strike_ = strike_;
  this->payoffCoeff_ = pnl_vect_copy(payoffCoeff_);
  this->lowerBarrier_ = pnl_vect_copy(lowerBarrier_);
}

OptionBarrierLow::~OptionBarrierLow(){
	pnl_vect_free(&(this->payoffCoeff_));
	pnl_vect_free(&(this->lowerBarrier_));
}

double OptionBarrierLow::payoff(const PnlMat *path)
{
}