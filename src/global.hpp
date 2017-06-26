#ifndef _GLOBAL_HPP_INCLUDED_
#define _GLOBAL_HPP_INCLUDED_

#include "particle.hpp"
#include "base/topology.hpp"

using namespace std;

class Global : public Topology
{
public:
	Global(float q=0.5) : Topology(q, 1, "Global") {};
	void initialize(vector<Particle>* swarm);
	void getBestPos(unsigned short n, vector<float>& pos);
	void update(unsigned short n, float score);
	static const char* name;
private:
	float bestScore;
	vector<float> bestPos;
	vector<Particle>* swarm;
};

#endif