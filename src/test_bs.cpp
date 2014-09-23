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
  BS *b;
  b = new BS(spot,sigma,rho,r,size);
  
  
  //TEST gaussien vector
  //Initiliaze the generator
  
  PnlRng *rng;
  rng= pnl_rng_create (PNL_RNG_MERSENNE);
  pnl_rng_sseed (rng, 0);
  PnlMat *path;
  path= pnl_mat_create(4,size);
  PnlMat *past_path;

  b->asset(path,3,3,rng);
  
  //Copy the result
  past_path= pnl_mat_copy(path);
  pnl_mat_free(&path);
  path= pnl_mat_create(6,size);
  b->asset(path,3,6,5,rng,past_path);
 
  //Test mc
  MonteCarlo *mc;
  mc= new MonteCarlo(P);
  double prix;
  double ic;
  //mc->price(prix,ic);
  cout<<"price à 0"<<prix<<endl;
  cout<<"ic à 0"<<ic<<endl;
  //Try with t=3
  //mc->price(past_path,3,prix,ic);

  cout<<"price à t"<<prix<<endl;
  cout<<"ic à t"<<ic<<endl;


  // Test for shift asset
  PnlMat* shift_path = pnl_mat_new();
  b->shift_asset(shift_path, path, path->n-1, 1, 3, 1);
  cout << "\n\n===============================================\nPath\n";
  pnl_mat_print(path);

  cout << "\n\n===============================================\nShifted path\n";
  pnl_mat_print(shift_path);

  PnlVect *delta = pnl_vect_create(path->n);
  mc->delta(past_path, 1, delta, NULL);
  cout << "\n\n===============================================\nDelta\n";
  pnl_vect_print(delta);

  pnl_vect_free(&delta);
  pnl_mat_free(&path);
  pnl_mat_free(&shift_path);
  pnl_mat_free(&past_path);
  pnl_rng_free(&rng);
  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  delete P;
  delete b;
  



  exit(0);
}
