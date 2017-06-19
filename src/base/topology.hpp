#ifndef _TOPOLOGY_HPP_INCLUDED_
#define _TOPOLOGY_HPP_INCLUDED_

class Particle;

using namespace std;

class Topology
{
public:
	virtual void initialize(vector<Particle>* swarm) = 0;
	virtual void getBestPos(unsigned short n, vector<float>& pos) = 0;
	virtual void update(unsigned short n, float score) = 0;
	float q;
	int k;
protected:
	Topology(float q, int k=1) : q(q), k(k) {};
};

#endif