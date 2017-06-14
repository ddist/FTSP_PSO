#include "particle.hpp"

Particle::Particle(unsigned short m, default_random_engine& gen, PSOParams& params) 
: position(3*m)
, velocity(3*m) /* Velocity initialized to 0 for all particles. */
, pBestScore(0)
, pBest(3*m)
{
	auto dist1 = uniform_real_distribution<float>(params.uMin, params.uMax);
	auto dist2 = uniform_real_distribution<float>(params.vMin, params.vMax);

	function<float()> gen1 = bind(dist1, gen);
	function<float()> gen2 = bind(dist2, gen);
	// Position and velocity initialized to uniformily distributed random values
	generate_n(this->position.begin(), 3*m, gen1);
	generate_n(this->velocity.begin(), 3*m, gen2);
	// Initialize pBest to the random particle position
	copy(this->position.begin(), this->position.end(), this->pBest.begin());
}

void Particle::updateBest(float score) {
	if(this->pBestScore < score) {
		this->pBestScore = score;
		copy(this->position.begin(), this->position.begin(), this->pBest.begin());
	}
}

void Particle::updateVelocity(function<float()>& generator, PSOParams& params, vector<float>& gBest) {
	float u1 = generator();
	float u2 = generator();

	for(size_t i=0; i<this->position.size(); i++) {
		this->velocity[i] = params.vWeight*this->velocity[i] + params.q1*u1*(this->pBest[i] - this->position[i]) + params.q2*u2*(gBest[i] - this->position[i]);
	}
}

void Particle::updatePosition(PSOParams& params) {
	for(size_t i=0; i<this->position.size(); i++) {
		this->position[i] += this->velocity[i];
		if(this->position[i] > params.uMax) this->position[i] = params.uMax;
		if(this->position[i] < params.uMin) this->position[i] = params.uMin;
	}
}