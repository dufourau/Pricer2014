#include <iostream>
#include "bs.h"
#include "parser.h"
#include "mc.h"
#include "pnl/pnl_finance.h"
using namespace std;

int main(int argc, char **argv)
{

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
  P->extract("strike",strike);
  double ptprice, ptdelta;
  pnl_cf_call_bs(GET(spot,0),strike,T,r,0.0,GET(sigma,0),&ptprice,&ptdelta);
  cout<< "\n\n" << ptprice <<"\n" <<ptdelta <<"\n\n"<<endl;
  //TEST Constructor
  PnlRng *rng;
  rng= pnl_rng_create (PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, 0);
  MonteCarlo *mc;
  mc= new MonteCarlo(P,2);
  PnlVect *V;
  V= pnl_vect_create(3);
  double profitLoss;
  mc->freeRiskInvestedPart(V,T,profitLoss);
  cout << "Price vector V: "<<endl;
  pnl_vect_print(V);
  cout << "Profit and Loss: "<< profitLoss<<endl;
  //b->simul_market(path,3,3,rng);
  //cout << "path"<<endl;
  //pnl_mat_print(path);
  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  exit(0);
}
