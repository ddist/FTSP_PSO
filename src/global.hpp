#ifndef _GLOBAL_HPP_INCLUDED_
#define _GLOBAL_HPP_INCLUDED_

#include "particle.hpp"
#include "base/topology.hpp"

using namespace std;

class Global : public Topology
{
public:
	Global(float q) : Topology(q) {};
	void initialize(vector<Particle>* swarm);
	void getBestPos(unsigned short n, vector<float>& pos);
	void update(unsigned short n, float score);
private:
	float bestScore;
	vector<float> bestPos;
	vector<Particle>* swarm;
};

#endif