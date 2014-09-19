#include "optionBarrierLow.h"
#include <assert.h> 
#include <iostream>

using namespace std;

OptionBarrierLow::OptionBarrierLow(const double T_, const int timeSteps_, const int size_, const double strike_, const PnlVect* payoffCoeff_, const PnlVect* lowerBarrier_) : Option(T_, timeSteps_, size_)
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
  assert(lowerBarrier_->size == path->n);
	for (int i = 0; i < path->m; ++i)
	{
    for (int d = 0; d < path->n; ++d)
    {
      if (MGET(path, i, d) < GET(this->lowerBarrier_, d)){
        return 0;
      }
    }
	}

  double res = 0.0;

  int i;
  PnlVect * assetAtMaturity = pnl_vect_new();

  //On extrait la ligne 
  pnl_mat_get_row(assetAtMaturity, path, (this->T_)-1); 
  
  assert(assetAtMaturity->size == payoffCoeff_->size);

  int size = assetAtMaturity->size;
  for(int i = 0; i<size; i++){
    res += GET(assetAtMaturity,i)*GET(payoffCoeff_,i);
  }

  res -= this->strike_;

  if(res<=0.0){
    return 0.0;
  }

  return res;

}