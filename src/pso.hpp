#include "instance.hpp"
#include <random>
#include <vector>
#include <functional>
#include <algorithm>

#ifndef _PSO_HPP_INCLUDED_
#define _PSO_HPP_INCLUDED_

using namespace std;

#define M 99999.9

struct PSOParams {
	float q1, q2; // Social and cognitive factors
	float uMax, uMin; // Min/Max uniform distribution position values
	float vMax, vMin; // Min/Max particle velocity values
	float wStart, wEnd; // Inertia values. Performs linear decay only when maxIter is set
	unsigned short nParticles; // Number of particles in the swarm
	unsigned int seed; // Seed value to feed random number generator
	/* TODO Topology params */
};

class Particle {
public:
	Particle(unsigned short n, unsigned short m, function<float()>& generator);
	vector<float>& getPos();
	//void updatePos(float q1, float q2, float r1, float r2, float w);
private:
	vector<float> position;
	vector<float> velocity;
	float pBestScore;
	vector<float> pBest;
};

vector<unsigned short> getPriorityList (vector<float>& pos, unsigned short nJobs);

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

	//void initialize();
	//void decode();
	//void evaluate();
};

#endif