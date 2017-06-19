#include "particle.hpp"

Particle::Particle(unsigned short m, default_random_engine& gen, PSOParams& params) 
: pBestScore(0)
, position(3*m)
, velocity(3*m)
, pBest(3*m)
{
	auto dist1 = uniform_real_distribution<float>(params.uMin, params.uMax);
	auto dist2 = uniform_real_distribution<float>(params.vMin, params.vMax);

	function<float()> gen1 = bind(dist1, ref(gen));
	function<float()> gen2 = bind(dist2, ref(gen));
	// Position and velocity initialized to uniformily distributed random values
	generate_n(this->position.begin(), 3*m, gen1);
	generate_n(this->velocity.begin(), 3*m, gen2);

	// Initialize pBest to the random particle position
	copy(this->position.begin(), this->position.end(), this->pBest.begin());
}

void Particle::updateBest(float score) {
	if(this->pBestScore < score) {
		this->pBestScore = score;
		copy(this->position.begin(), this->position.end(), this->pBest.begin());
	}
}

void Particle::updateVelocity(Topology **t, int tn, int n, float w, function<float()>& generator, PSOParams& params) {
	vector<float> u = vector<float>(tn+1);
	generate_n(u.begin(), tn+1, generator);

	vector<vector<float>> tBest = vector<vector<float>>(tn, vector<float>(this->position.size()));

	for(int i=0;i<tn;i++) {
		t[i]->getBestPos(n, tBest[i]);
	}

	for(size_t i=0; i<this->position.size(); i++) {
		this->velocity[i] = w*this->velocity[i] + params.q*u[0]*(this->pBest[i] - this->position[i]);
		for(int j=0;j<tn;j++) {
			this->velocity[i] += t[j]->q*u[j+1]*(tBest[j][i] - this->position[i]);
		}
		if(this->velocity[i] > params.vMax) {this->velocity[i] = params.vMax;};
		if(this->velocity[i] < params.vMin) {this->velocity[i] = params.vMin;};
	}
}

void Particle::updatePosition(PSOParams& params) {
	for(size_t i=0; i<this->position.size(); i++) {
		this->position[i] += this->velocity[i];
		if(this->position[i] > params.uMax) this->position[i] = params.uMax;
		if(this->position[i] < params.uMin) this->position[i] = params.uMin;
	}
}