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
	FDR(float q=1.5) : Topology(q, 1, "FDR") {};
	void initialize(vector<Particle>* swarm);
	void getBestPos(unsigned short n, vector<float>& pos);
	void update(unsigned short n, float score);
	static const char* name;
private:
	vector<Particle>* swarm;
};

#endif