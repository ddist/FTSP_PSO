#ifndef _PSO_HPP_INCLUDED_
#define _PSO_HPP_INCLUDED_

#include <random>
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <cstdlib>
#include "instance.hpp"
#include "pso_params.hpp"
#include "particle.hpp"
#include "h_priority.hpp"
#include "n_global.hpp"

using namespace std;

class PSO {
public:
	PSO(Instance& inst, PSOParams& params);
	void solve();
private:
	vector<Particle> swarm;
	float gBestScore;
	vector<float> gBest;

	default_random_engine generator;

	Instance instance;
	PSOParams parameters;
};

#endif