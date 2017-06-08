#include "pso.hpp"

/* 2D Euclidian distance */
float norm1(float x1, float x2, float y1, float y2) {
	return sqrt( pow(x1 - y1, 2) + pow(x2 - y2, 2));
}

void getCoverageMatrix(vector<float>& pos, Instance& inst, vector<vector<unsigned short>>& coverage) {
	for(int i=0;i<inst.m;i++) {
		for(int j=0;j<inst.n;j++) {
			// Check if technician has the required skill
			if(inst.s[j+1][i]) {
				// Check if job is in technician range (euclidian distance)
				if(norm1(pos[3*i], pos[3*i+1], inst.c[j+1][0], inst.c[j+1][1]) <= pos[3*i+2]) {
					coverage[j][i] = inst.w[j+1];
				}
			}
		}
	}
}

void getDistanceMatrix(Instance& inst, vector<vector<float>>& distances) {
	for(int i=0;i<inst.N;++i) {
		for (int j=0;j<inst.N;++j) {
			if(i==j) {
				distances[i][j] = 0.0;
			} else if (i>j) {
				distances[i][j] = distances[j][i];
			} else {
				distances[i][j] = norm1(inst.c[i][0], inst.c[i][1], inst.c[j][0], inst.c[j][1]);
			}
		}
	}
}

void decode(Particle& particle, vector<vector<unsigned short>>& routes) {
	return;
}

float evaluate(vector<vector<unsigned short>>& routes) {
	return 0.0;
}

Particle::Particle(unsigned short m, function<float()>& generator) 
: position(3*m)
, velocity((3*m), 0) /* Velocity initialized to 0 for all particles. */
, pBestScore(0)
, pBest(3*m)
{
	// Position initialized to uniformily distributed random values
	generate_n(this->position.begin(), 3*m, generator);
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

	for(int i=0; i<this->position.size(); i++) {
		this->velocity[i] += (params.q1*u1*(this->pBest[i]-this->position[i]) + params.q2*u2*(gBest[i] - this->position[i]));
		if(this->velocity[i] > params.vMax) this->velocity[i] = params.vMax;
		if(this->velocity[i] < params.vMin) this->velocity[i] = params.vMin;
	}
}

void Particle::updatePosition(PSOParams& params) {
	for(int i=0; i<this->position.size(); i++) {
		this->position[i] = this->position[i] + this->velocity[i];
		if(this->position[i] > params.uMax) this->position[i] = params.uMax;
		if(this->position[i] < params.uMin) this->position[i] = params.uMin;
	}
}


PSO::PSO(Instance& inst, PSOParams& params)
: gBestScore(0)
, gBest(3*inst.m)
, instance(inst)
, parameters(params)
{
	this->generator = default_random_engine(params.seed);
	uniform_real_distribution<float> dist(params.uMin, params.uMax);
	function<float()> gen = bind(dist, this->generator);
	this->swarm = vector<Particle>(params.nParticles, Particle(inst.m, gen));
}

void PSO::solve() {

	vector<vector<float>> distances = vector<vector<float>>(this->instance.N, vector<float>(this->instance.m, 0.0));
	getDistanceMatrix(this->instance, distances);

	unsigned short nIter = 0;

	uniform_real_distribution<float> dist(0.0,1.0);
	function<float()> gen = bind(dist, this->generator);

	do 
	{
		for(int i=0; i < this->parameters.nParticles; i++) {
			// Decode particle position into technician routes
			vector<vector<unsigned short>> routes = vector<vector<unsigned short>>(this->instance.m, vector<unsigned short>());
			decode(this->swarm[i], routes);
			// Evaluate the candidate solution
			float score = evaluate(routes);
			// Update particle's best scores
			this->swarm[i].updateBest(score);
			// Update neighborhood's best scores
			if(this->gBestScore < score) {
				this->gBestScore = score;
				copy(this->swarm[i].position.begin(), this->swarm[i].position.end(), this->gBest.begin());
			}
			// Update particle velocity and position
			this->swarm[i].updateVelocity(gen, this->parameters, this->gBest);
			this->swarm[i].updatePosition(this->parameters);
		}

		nIter++;

	} while (nIter < this->parameters.maxIter);

}