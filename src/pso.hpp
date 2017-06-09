#include "instance.hpp"
#include <random>
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <math.h>

#ifndef _PSO_HPP_INCLUDED_
#define _PSO_HPP_INCLUDED_

using namespace std;

struct PSOParams {
	float q1, q2; // Social and cognitive factors
	float uMax, uMin; // Min/Max uniform distribution position values
	float vMax, vMin; // Min/Max particle velocity values
	float vWeight; // Inertia value
	unsigned short nParticles; // Number of particles in the swarm
	unsigned int seed; // Seed value to feed random number generator
	unsigned short maxIter = 100; // maximum number of iterations as a stopping criteria
	/* TODO Topology params */
};

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