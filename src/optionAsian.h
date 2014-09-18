#ifndef _OPTIONASIAN_H
#define _OPTIONASIAN_H

#include "option.h"

class OptionAsian : 
	public Option
{
public:
	double strike_;
	PnlVect *payoffCoeff_;

	OptionAsian(double strike_, PnlVect *payoffCoeff_,double T_, int TimeSteps_, int size_);

	~OptionAsian();

	double payoff(const PnlMat *path);
};

#endif //_OPTIONASIAN_H