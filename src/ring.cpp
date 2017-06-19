#include "ring.hpp"

void Ring::initialize(vector<Particle>* swarm) {
	this->swarm = swarm;
}

void Ring::getBestPos(unsigned short n, vector<float>& pos) {
	int best;
	float score = 0.0;

	for(int i = n-this->k; i <= n+this->k;i++) {
		float temp = this->swarm[0][i%this->swarm[0].size()].pBestScore;
		if(temp > score) {
			score = temp;
			best = i%this->swarm[0].size();
		}
	}
	copy(this->swarm[0][best].position.begin(), this->swarm[0][best].position.end(), pos.begin());
}



void Ring::update(unsigned short n, float score) {
	return;
}