#ifndef _OPTIONBARRIERLOW_H
#define _OPTIONBARRIERLOW_H

#include "option.h"

class OptionBarrierLow : public Option
{
public:
  double strike_;
  PnlVect* payoffCoeff_;
  PnlVect* lowerBarrier_;

  OptionBarrierLow(double T_, int timeSteps_, int size_, double strike_, PnlVect* payoffCoeff_, PnlVect* lowerBarrier_);

  double payoff(const PnlMat *path);

  ~OptionBarrierLow();
};

#endif //_OPTIONBARRIERLOW_H
