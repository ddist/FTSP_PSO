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
	float wStart, wEnd; // Inertia values. Performs linear decay only when maxIter is set
	unsigned short nParticles; // Number of particles in the swarm
	unsigned int seed; // Seed value to feed random number generator
	/* TODO Topology params */
};

/*struct Solution {
	vector<vector<unsigned short>> routes;
	vector<float> t;
	vector<float> z;

	float score;
};*/

class Particle {
public:
	Particle(unsigned short m, function<float()>& generator);
	//vector<vector<unsigned short>> decode();

	vector<float> position;
private:
	vector<float> velocity;
	float pBestScore;
	vector<float> pBest;
};

class PSO {
public:
	PSO(Instance& inst, PSOParams& params);
private:
	vector<Particle> swarm;
	float gBestScore;
	vector<float> gBest;

	function<float()> generator;

	Instance instance;
	PSOParams parameters;
};

#endif