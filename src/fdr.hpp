#ifndef _FDR_HPP_INCLUDED_
#define _FDR_HPP_INCLUDED_

#include <limits>
#include <cstdlib>
#include "particle.hpp"
#include "base/topology.hpp"

using namespace std;

class FDR : public Topology
{
public:
	FDR(float q) : Topology(q) {};
	void initialize(vector<Particle>* swarm);
	void getBestPos(unsigned short n, vector<float>& pos);
	void update(unsigned short n, float score);
private:
	vector<Particle>* swarm;
};

#endif