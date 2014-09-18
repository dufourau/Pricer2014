#include <iostream>

using namespace std;

#include "optionAsian.h"

OptionAsian::OptionAsian(double strike_, PnlVect *payoffCoeff_,double T_, int TimeSteps_, int size_):Option(T_,TimeSteps_,size_)
{
	this->strike_=strike_;
	this->payoffCoeff_=pnl_vect_copy(payoffCoeff_);
}

OptionAsian::~OptionAsian()
{
	pnl_vect_free(&(payoffCoeff_));
}

double OptionAsian::payoff(const PnlMat *path)
{

}