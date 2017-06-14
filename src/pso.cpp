#include "pso.hpp"

float evaluate(vector<vector<unsigned short>>& routes, vector<float>& zk, Instance& instance) {
	float score = 0.0;
	float w = 0.0, z = 0.0;
	for(size_t i=0;i<routes.size();i++) {
		// Score is 0 if a technician does not leave the origin
		if(routes[i].size() < 3) {
			w = 0.0; z = 0.0;
			//cout << "0-0";
			break;
		}
		// Sum priorities
		for(size_t j=0;j<routes[i].size();j++) {
			w += (float)instance.w[routes[i][j]];
			//cout << routes[i][j] << "-";
		}
		//cout << " " << zk[i] << " | ";
		// Sum free time
		z += zk[i];
	}
	score = w/(float)instance.MW + z/(float)instance.MZ;
	//cout << score << endl;
	return score;
}


PSO::PSO(Instance& inst, PSOParams& params)
: gBestScore(0)
, gBest(3*inst.m, 0.0)
, instance(inst)
, parameters(params)
{
	this->generator = default_random_engine(params.seed);
	this->swarm = vector<Particle>(params.nParticles, Particle(inst.m, this->generator, params));
}

void PSO::solve() {

	HPriority decoder = HPriority();
	decoder.initialize(this->instance);

	unsigned short nIter = 0;

	uniform_real_distribution<float> dist(0.0,1.0);
	function<float()> gen = bind(dist, this->generator);

	do 
	{
		for(int i=0; i < this->parameters.nParticles; i++) {
			// Decode particle position into technician routes
			auto routes = vector<vector<unsigned short>>(this->instance.m, vector<unsigned short>(2, 0));
			auto zk = vector<float>(this->instance.m, 0.0);
			
			decoder.decode(this->swarm[i], routes, zk);
			// Evaluate the candidate solution
			float score = evaluate(routes, zk, this->instance);
			// Update particle's best score
			this->swarm[i].updateBest(score);
			// Update neighborhood's best score
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

	cout << "Best score: " << this->gBestScore << endl;

}