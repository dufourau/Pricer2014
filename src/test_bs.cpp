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
  path= pnl_mat_create(4,size);
  PnlMat *past_path;

  for(int j=0; j<size; j++){
      MLET(path,0,j)= GET(spot,j);
  }

  b->asset(path,3,3,rng);
  cout << "path matrix with call function baton:"<<endl;
  pnl_mat_print(path);

  //Copy the result
  past_path= pnl_mat_copy(path);
  pnl_mat_free(&path);
  path= pnl_mat_create(6,size);
  b->asset(path,3,6,5,rng,past_path);
  cout << "path matrix with call function autre:"<<endl;
  pnl_mat_print(path);

  pnl_mat_free(&path);
  pnl_mat_free(&past_path);
  pnl_rng_free(&rng);
  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  delete P;
  delete b;

  exit(0);
}
