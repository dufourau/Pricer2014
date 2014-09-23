#include <iostream>
#include "bs.h"
#include "parser.h"
#include "mc.h"
using namespace std;

int main(int argc, char **argv)
{

  double rho;
  PnlVect *spot, *sigma, *trend;
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
  trend=pnl_vect_create(size);
  for(int i=0;i<size;i++){
    LET(trend,i)=0.1;
  }
  //TEST Constructor
  BS *b;
  b = new BS(spot,sigma,rho,r,size,trend);
  
  

  //TEST gaussien vector
  //Initiliaze the generator
  
  PnlRng *rng;
  rng= pnl_rng_create (PNL_RNG_MERSENNE);
  pnl_rng_sseed (rng, 0);
  PnlMat *path;
  path= pnl_mat_create(4,size);

  b->simul_market(path,3,3,rng);
  cout << "path matrix with call function baton:"<<endl;
  pnl_mat_print(path);

  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  pnl_vect_free(&trend);
  exit(0);
}
