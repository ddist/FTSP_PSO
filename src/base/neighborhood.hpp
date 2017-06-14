#ifndef _NEIGHBORHOOD_HPP_INCLUDED_
#define _NEIGHBORHOOD_HPP_INCLUDED_

#include "../particle.hpp"

using namespace std;

class Neighborhood
{
public:
	virtual void initialize(vector<Particle>& swarm) = 0;
	virtual void getBestPos(unsigned short n, vector<float>& pos) = 0;
	virtual void update(unsigned short n, float score) = 0;
};

#endif