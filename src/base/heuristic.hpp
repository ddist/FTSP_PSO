#ifndef _HEURISTIC_HPP_INCLUDED_
#define _HEURISTIC_HPP_INCLUDED_

#include "../particle.hpp"
#include "../instance.hpp"

using namespace std;

class Heuristic 
{
public:
	virtual void initialize(Instance& instance) = 0;
	virtual void decode(Particle& particle, vector<vector<unsigned short>>& routes, vector<float>& zk) = 0;
};

#endif