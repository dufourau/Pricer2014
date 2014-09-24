#include "../../src/bs.h"
#include "../../src/parser.h"
#include "../../src/mc.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char const *argv[])
{
  const char *infile = argv[1];
  double t;
  if (argv[2])
    t = atof(argv[2]);

  Param *P = new Parser(infile);
  Option* opt = MonteCarlo::createOption(P);
  // Initialize BS parameters
  double rho;
  PnlVect *spot, *sigma;
  double T, r, strike;
  int size, tNumber;
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("spot", spot, size);
  P->extract("volatility", sigma, size);
  P->extract("interest rate", r);
  P->extract("correlation",rho);
  P->extract("timestep number",tNumber);
  
  //Initiliaze the generator
  PnlRng *rng;
  rng= pnl_rng_create (PNL_RNG_MERSENNE);
  pnl_rng_sseed (rng, 0);
  PnlVect *delta;

  int N= (int) ceil(t/(T/tNumber));
  PnlMat* past_path;
  past_path= pnl_mat_create(N+1,size);
  
  MonteCarlo *mc = new MonteCarlo(P);
  mc->mod_->asset(past_path,t,N,rng);

  delta= pnl_vect_create(size);

  if (t <= 0 || t > T){
    cout << "t is not between 0 and " << T << ", computing delta for t = 0" << endl;
    t = 0;
  }

  mc->delta(past_path,t,delta,NULL);

  cout << "Delta :" << endl;
  pnl_vect_print(delta);

  delete opt;
  delete P;
  delete mc;
  return 0;
}
