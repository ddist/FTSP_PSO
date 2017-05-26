#include "pso.hpp"

/* 2D Euclidian distance */
float norm1(float x1, float x2, float y1, float y2) {
	return sqrt( pow(x1 - y1, 2) + pow(x2 - y2, 2));
}

void getCoverageMatrix(vector<float>& pos, Instance& inst, vector<vector<bool>>& coverage) {
	for(int i=0;i<inst.m;i++) {
		for(int j=0;j<inst.n;j++) {
			// Check if technician has the required skill
			if(inst.s[j+1][i]) {
				// Check if job is in technician range (euclidian distance)
				if(norm1(pos[3*i], pos[3*i+1], inst.c[j+1][0], inst.c[j+1][1]) <= pos[3*i+2]) {
					coverage[j][i] = true;
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

Particle::Particle(unsigned short m, function<float()>& generator) 
: position(3*m)
, velocity((3*m), 0) /* Velocity initialized to 0 for all particles. */
, pBestScore(0)
, pBest(3*m)
{
	// Position initialized to uniformily distributed random values
	generate_n(this->position.begin(), 3*m, generator); 
}


PSO::PSO(Instance& inst, PSOParams& params)
: gBestScore(0)
, gBest(3*inst.m)
, instance(inst)
, parameters(params)
{
	default_random_engine gen(params.seed);
	uniform_real_distribution<float> dist(params.uMin, params.uMax);
	this->generator = bind(dist, gen);
	this->swarm = vector<Particle>(params.nParticles, Particle(inst.m, this->generator));
	vector<vector<bool>> matrix(inst.n, vector<bool>(inst.m, false));
	getCoverageMatrix(this->swarm[0].position, this->instance, matrix);
	vector<vector<float>> distances(inst.N, vector<float>(inst.N));
	getDistanceMatrix(inst, distances);
}