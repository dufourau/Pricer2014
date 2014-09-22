#include <iostream>
#include "mc.h"
#include "parser.h"


using namespace std;

int main(int argc, char **argv)
{

   //Initialize MC parameters
  double rho;
  PnlVect *spot, *sigma;
  double T, r, strike;
  int size;
  char *infile = argv[1];
  Param *P = new Parser(infile);

  MonteCarlo mc = MonteCarlo(P);

  //cout<< <<endl;  
//  cout<<"Nb tirage mc"<<mc.samples_<<endl; 
//  cout<<"Taille :"<<mc.mod_->size_<<endl; 
//  cout<<"Taux interet :"<<mc.mod_->r_<<endl; 
//  cout<<"Coeff coreation :"<<mc.mod_->rho_<<endl; 
//  
//  cout<<"Sigma"<<endl;
//  pnl_vect_print(mc.mod_->sigma_);
//  cout<<"spot"<<endl;
//  pnl_vect_print(mc.mod_->spot_);
//  
//   
//  cout<<"Strike"<<((OptionBasket *)mc.opt_)->strike_<<endl;
//  cout<<"PayOff Coeff"<<endl;
//  pnl_vect_print(((OptionBasket *)mc.opt_)->payoffCoeff_);


  double prix;
  double ic;
  
  mc.price(prix, ic);

  exit(0);
}
