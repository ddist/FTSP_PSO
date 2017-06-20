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
#include "global.hpp"
#include "ring.hpp"
#include "fdr.hpp"
#include "priority.hpp"
#include "urgency.hpp"
#include "particle.hpp"

using namespace std;

class PSO {
public:
	PSO(Instance& inst, PSOParams& params, Heuristic* h, int tn, Topology** t);
	void solve();
private:
	vector<Particle> swarm;
	float bestScore;

	default_random_engine generator;

	Instance instance;
	PSOParams parameters;

	Heuristic *heuristic;

	int tn;
	Topology **topologies;
};

#endif