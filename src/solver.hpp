#include "instance.hpp"
#include <random>
#include <vector>

#ifndef _SOLVER_HPP_INCLUDED_
#define _SOLVER_HPP_INCLUDED_

using namespace std;

class FTSPSolver {
	public:
		void solve(Instance* instance, ParameterSet* params);
	private:
		void initialize();
		void decode();
		void evaluate();

};

#endif