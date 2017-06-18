#include "global.hpp"

void Global::initialize(vector<Particle>* swarm) {
	this->swarm = swarm;
	this->bestScore = 0.0;
	this->bestPos = vector<float>(this->swarm[0][0].position.size(), 0.0);
}

void Global::getBestPos(unsigned short n, vector<float>& pos) {
	copy(this->bestPos.begin(), this->bestPos.end(), pos.begin());
}

void Global::update(unsigned short n, float score) {
	if(this->bestScore < score) {
		this->bestScore = score;
		copy(this->swarm[0][n].position.begin(), this->swarm[0][n].position.end(), this->bestPos.begin());
	}
}