#include <iostream>
#include "bs.h"
#include "parser.h"
#include "mc.h"
using namespace std;

int main(int argc, char **argv)
{

   //Initialize BS parameters
  double rho;
  PnlVect *spot, *sigma;
  double T, r, strike;
  int size;
  char *infile = argv[1];
  Param *P = new Parser(infile);
 
  
  
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("spot", spot, size);
  P->extract("volatility", sigma, size);
  P->extract("interest rate", r);
  P->extract("correlation",rho);
  
  //TEST gaussien vector
  //Initiliaze the generator
  
  PnlRng *rng;
  rng= pnl_rng_create (PNL_RNG_MERSENNE);
  pnl_rng_sseed (rng, 0);
  PnlMat *path;
  path= pnl_mat_create(7,size);
  PnlMat *past_path;
  //Test mc
  MonteCarlo *mc;
  mc= new MonteCarlo(P);
  double prix;
  double ic;

  mc->mod_->asset(path,1,6,rng);
  cout<<"path avant"<<endl; 
  //Copy the result
  past_path= pnl_mat_copy(path);
  pnl_mat_free(&path);
  pnl_mat_print(past_path);
  PnlVect *delta;
  delta= pnl_vect_create(size);
  
  mc->delta(past_path,0.5, delta, NULL);
  pnl_vect_print(delta);
  
  pnl_vect_free(&delta);
  pnl_mat_free(&path);
  pnl_mat_free(&past_path);
  pnl_rng_free(&rng);
  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  delete P;
  //delete b;
  delete mc;



  exit(0);
}
