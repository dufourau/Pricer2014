#include <iostream>
#include "bs.h"
#include "parser.h"
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


  //TEST Constructor
  BS *b;
  b = new BS(spot,sigma,rho,r,size);
  /*
  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  */
  //TEST gaussien vector
  //Initiliaze the generator
  PnlRng *rng;
  rng= pnl_rng_create (PNL_RNG_MERSENNE);
  pnl_rng_sseed (rng, 0);
  PnlMat *path;
  path= pnl_mat_create(21,size);
  PnlMat *past_path;
  past_path= pnl_mat_new();

  for(int j=0; j<size; j++){
      cout<<j<<endl;
      MLET(path,0,j)= GET(spot,j);
  }

  b->asset(path,T,20,rng);
  cout << "path matrix"<<endl;
  pnl_mat_print(path);
  pnl_rng_free(&rng);


  exit(0);
}
