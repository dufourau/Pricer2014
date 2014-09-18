#include <iostream>

using namespace std;

#include "option.h"

Option::Option(double T_, int TimeSteps_, int size_){
  this->T_ = T_;
  this->TimeSteps_ = TimeSteps_;
  this->size_ = size_;
}

Option::~Option(){}

