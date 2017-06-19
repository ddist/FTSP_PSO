#include "fdr.hpp"

float distance(vector<float>& x, vector<float>& y) {
	float sum = 0.0;
	for(size_t i=0;i<x.size();i++) {
		sum += (x[i] - y[i]);
	}
	return abs(sum);
}

void FDR::initialize(vector<Particle>* swarm) {
	this->swarm = swarm;
}

void FDR::getBestPos(unsigned short n, vector<float>& pos) {
	int best;
	float bestFDR = numeric_limits<float>::lowest();
	float FDR;

	for(size_t i=0;i < this->swarm[0].size();i++) {
		if(i==n) {continue;}
		FDR = (this->swarm[0][i].pBestScore - this->swarm[0][n].pBestScore)/distance(this->swarm[0][i].position, this->swarm[0][n].position);
		if(FDR > bestFDR) {
			bestFDR = FDR;
			best = i; 
		}
	}
	copy(this->swarm[0][best].position.begin(), this->swarm[0][best].position.end(), pos.begin());
}

void FDR::update(unsigned short n, float score) {
	return;
}