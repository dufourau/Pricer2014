#include "gtest/gtest.h"
#include "../src/optionBarrierLow.h"
#include <iostream>

TEST (OptionBarrierLowTest, Payoff) {
	PnlVect* p = pnl_vect_new();
	OptionBarrierLow* o = new OptionBarrierLow(0, 0, 0, 0, p, p);
}