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


  //TEST Constructor
  //BS *b;
  //b = new BS(spot,sigma,rho,r,size,trend);
  
  
  //TEST gaussien vector
  //Initiliaze the generator
  
  PnlRng *rng;
  rng= pnl_rng_create (PNL_RNG_MERSENNE);
  pnl_rng_sseed (rng, 0);
  PnlMat *path;
  path= pnl_mat_create(7,size);
  PnlMat *past_path;

  /*b->asset(path,1,6,rng);
  cout<<"path avant"<<endl;
  pnl_mat_print(path);
  //Copy the result
  past_path= pnl_mat_copy(path);
  pnl_mat_free(&path);
  //path= pnl_mat_create(13,size);
  //b->asset(path,3,6,5,rng,past_path);
  */
  //Test mc
  MonteCarlo *mc;
  mc= new MonteCarlo(P);
  double refprix;
  double refic;
  
  
  mc->price(refprix,refic);
  cout<<"price à 0"<<refprix<<endl;
  cout<<"ic à 0"<<refic<<endl;
  
  
  //Try with t=3

  //mc->price(past_path,0.6,refprix,refic);
  //cout<<"price à t"<<refprix<<endl;
  //cout<<"ic à t"<<refic<<endl;
  
  // Test for shift asset
  BS* bs = new BS(spot,sigma,rho,r,size,NULL);
  PnlMat* shift_path = pnl_mat_new();

  path= pnl_mat_create(4,size);
  past_path = pnl_mat_copy(path);
  bs->asset(path,3,6,5,rng,past_path);
  bs->shift_asset(shift_path, path, path->n-1, 1, 3, 1);
  cout << "\n\n===============================================\nPath\n";
  pnl_mat_print(path);

  cout << "\n\n===============================================\nShifted path\n";
  pnl_mat_print(shift_path);

  PnlVect *delta = pnl_vect_create(path->n);
  mc->delta(past_path, 1, delta, NULL);
  cout << "\n\n===============================================\nDelta\n";
  pnl_vect_print(delta);

  //pnl_mat_free(&past_path);
  pnl_rng_free(&rng);
  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  delete P;
  //delete b;
  delete mc;



  exit(0);
}
