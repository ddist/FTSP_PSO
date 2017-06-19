#include "pso.hpp"

float evaluate(vector<vector<unsigned short>>& routes, vector<float>& zk, Instance& instance) {
	float score = 0.0;
	float w = 0.0, z = 0.0;
	for(size_t i=0;i<routes.size();i++) {
		// Score is 0 if a technician does not leave the origin
		if(routes[i].size() < 3) {
			w = 0.0; z = 0.0;
			break;
		}
		// Sum priorities
		for(size_t j=0;j<routes[i].size();j++) {
			w += (float)instance.w[routes[i][j]];
		}
		// Sum free time
		z += zk[i];
	}
	score = w/(float)instance.MW + z/(float)instance.MZ;
	return score;
}


PSO::PSO(Instance& inst, PSOParams& params, Heuristic* h, int tn, Topology** t)
: bestScore(0)
, instance(inst)
, parameters(params)
, heuristic(h)
, tn(tn)
, topologies(t)
{
	this->generator = default_random_engine(params.seed);
	for(int i=0;i < params.nParticles;i++) {
		this->swarm.push_back(Particle(inst.m, ref(this->generator), params));
	}

}

void PSO::solve() {

	this->heuristic->initialize(this->instance);
	for(int i=0;i<this->tn;i++) {
		this->topologies[i]->initialize(&(this->swarm));
	}

	unsigned short nIter = 0;

	float w = this->parameters.wMax;

	uniform_real_distribution<float> dist(0.0,1.0);
	function<float()> gen = bind(dist, ref(this->generator));

	do 
	{
		for(int i=0; i < this->parameters.nParticles; i++) {
			// Decode particle position into technician routes
			auto routes = vector<vector<unsigned short>>(this->instance.m, vector<unsigned short>(2, 0));
			auto zk = vector<float>(this->instance.m, 0.0);
			
			this->heuristic->decode(this->swarm[i], routes, zk);
			// Evaluate the candidate solution
			float score = evaluate(routes, zk, this->instance);
			// Update particle's best score
			this->swarm[i].updateBest(score);
			//Update neighborhood best
			for(int j=0;j<this->tn;j++) {this->topologies[j]->update((unsigned short)i, score);};
			// Update PSO best scor
			if(this->bestScore < score) {
				this->bestScore = score;
			}
			// Update particle velocity and position
			this->swarm[i].updateVelocity(this->topologies, this->tn, i, w, gen, this->parameters);
			this->swarm[i].updatePosition(this->parameters);
			// Update inertia
			w = (w - this->parameters.wMin)*(this->parameters.maxIter - nIter)/(this->parameters.maxIter+this->parameters.wMin);
		}

		nIter++;

	} while (nIter < this->parameters.maxIter);

	cout << "Best score: " << this->bestScore << endl;
}