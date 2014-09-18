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

  //Call Constructor
  BS *b;
  rho= 1.1;
  r= 2.1;
  size=6;
  b = new BS(spot,sigma,rho,r,size);
  //Print Attributes
  cout << "rho " << b->rho_ << endl;
  cout << "r " << b->r_ << endl;
  cout << "size " <<  b->size_ << endl;
  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  //
  exit(0);
}
