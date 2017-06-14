#include "n_global.hpp"

void NGlobal::initialize(vector<Particle>& swarm) {
	this->swarm = swarm;
}

void NGlobal::getBestPos(unsigned short n, vector<float>& pos) {
	copy(this->gBestPos.begin(), gBestPos.end(), pos.begin());
}

void NGlobal::update(unsigned short n, float score) {
	if(score > this->gBestScore) {
		this->gBestScore = score;
		copy(this->swarm[n].position.begin(), this->swarm[n].position.end(), this->gBestPos.begin());
	}
}