#ifndef _N_GLOBAL_HPP_INCLUDED_
#define _N_GLOBAL_HPP_INCLUDED_

#include "base/neighborhood.hpp"

using namespace std;

class NGlobal : public Neighborhood
{
public:
	void initialize(vector<Particle>& swarm);
	void getBestPos(unsigned short n, vector<float>& pos);
	void update(unsigned short n, float score);

	float gBestScore;
	vector<float> gBestPos;
private:
	vector<Particle> swarm;
	vector<vector<unsigned short>> neighborMatrix;
};

#endif