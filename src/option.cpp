#include <iostream>

using namespace std;

#include "option.h"

Option::Option(double T_, int timeSteps_, int size_){
  this.T_ = T_;
  this.timeSteps_ = timeSteps_;
  this.size_ = size_;
}

Option::~Option(){}

