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
	if (&(payoffCoeff_) != NULL)
		pnl_vect_free(&(payoffCoeff_));
}

double OptionAsian::payoff(const PnlMat *path)
{
	// Put the first column of path in payoffCoeff_
	PnlVect *p= pnl_vect_new();
	pnl_mat_get_col(p,path,0);
	double sum_flow = 0.0;
	for (int i=0;i<p->size;i++){
		sum_flow+=GET(p,i);
	}
	pnl_vect_free(&p);
	double payoff = (sum_flow/this->TimeSteps_)-(this->strike_);
	
	if (payoff<0.0) {
		return 0.0;
	}else{
		return payoff;
	}
}