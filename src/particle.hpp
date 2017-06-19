#ifndef _PARTICLE_HPP_INCLUDED_
#define _PARTICLE_HPP_INCLUDED_

#include <random>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include "pso_params.hpp"
#include "base/topology.hpp"

using namespace std;

class Particle {
public:
	Particle(unsigned short m, default_random_engine& gen, PSOParams& params);
	float pBestScore;
	vector<float> position;

	void updateBest(float score);
	void updateVelocity(Topology** t, int tn, int n, float w, function<float()>& generator, PSOParams& params);
	void updatePosition(PSOParams& params);
private:
	vector<float> velocity;
	vector<float> pBest;
};

#endif