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
  PnlMat *path;
  MonteCarlo *mc = new MonteCarlo(P,4);

  double prix;
  double ic;

  if (t <= 1e-10 || t > T){
    mc->price(prix, ic);
  } else {
    int N= (int) ceil(t/(T/tNumber));
    PnlMat* path;
    path= pnl_mat_create(N+1,size);
    mc->mod_->asset(path,t,N,rng);

    mc->price(path , t, prix, ic);
  }

  mc->price(prix, ic);

  cout << "Output : " << endl << "Price : " << prix << endl << "Confidence interval : " << ic << endl;

  delete opt;
  delete P;
  delete mc;
  return 0;
}
