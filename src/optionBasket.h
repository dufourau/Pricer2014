#ifndef _OPTIONBASKET_H
#define _OPTIONBASKET_H

#include "option.h"

class OptionBasket : public Option
{
public:
  double strike_;
  PnlVect * payoffCoeff_;

  OptionBasket(double T_, int timeSteps_, int size_, double strike_, PnlVect* payoffCoeff);
  
  ~OptionBasket();

  double payoff(const PnlMat *path);
};

#endif //_OPTIONBASKET_H
