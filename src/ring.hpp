#ifndef _RING_HPP_INCLUDED_
#define _RING_HPP_INCLUDED_

#include "particle.hpp"
#include "base/topology.hpp"

using namespace std;

class Ring : public Topology
{
public:
	Ring(float q, int k=1) : Topology(q, k) {};
	void initialize(vector<Particle>* swarm);
	void getBestPos(unsigned short n, vector<float>& pos);
	void update(unsigned short n, float score);
private:
	vector<Particle>* swarm;
};

#endif