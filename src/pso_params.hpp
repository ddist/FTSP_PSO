#ifndef _PSO_PARAMS_HPP_INCLUDED_
#define _PSO_PARAMS_HPP_INCLUDED_

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

#endif