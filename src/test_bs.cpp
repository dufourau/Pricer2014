#include <iostream>
#include "bs.h"

using namespace std;

int main(int argc, char **argv)
{
  //Initialize BS parameters
  double rho, r;
  PnlVect *spot, *sigma;
  int size;
  spot= pnl_vect_new ();
  sigma= pnl_vect_new ();

  //TEST Constructor
  BS *b;
  rho= 0.3;
  r= 2.1;
  size=6;
  b = new BS(spot,sigma,rho,r,size);
  //Print Attributes
  cout << "rho " << b->rho_ << endl;
  cout << "r " << b->r_ << endl;
  cout << "size " <<  b->size_ << endl;
  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  //TEST gaussien vector
  //Initiliaze the generator
  PnlRng *rng;
  rng= pnl_rng_create (PNL_RNG_MERSENNE);
  pnl_rng_sseed (rng, 0);

  PnlMat *path;
  path= pnl_mat_new();
  PnlMat *past_path;
  past_path= pnl_mat_new();
  b->asset(path,1,2,3,rng,past_path);
  
  pnl_rng_free(&rng);


  exit(0);
}
