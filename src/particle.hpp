#ifndef _PARTICLE_HPP_INCLUDED_
#define _PARTICLE_HPP_INCLUDED_

#include <random>
#include <functional>
#include <vector>
#include <algorithm>
#include "pso_params.hpp"

using namespace std;

class Particle {
public:
	Particle(unsigned short m, default_random_engine& gen, PSOParams& params);
	vector<float> position;

	void updateBest(float score);
	void updateVelocity(function<float()>& generator, PSOParams& params, vector<float>& gBest);
	void updatePosition(PSOParams& params);
private:
	vector<float> velocity;
	float pBestScore;
	vector<float> pBest;
};

#endif