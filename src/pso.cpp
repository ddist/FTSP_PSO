#include "pso.hpp"

vector<float>& Particle::getPos() {
	return this->position;
}

Particle::Particle(unsigned short n, unsigned short m, function<float()>& generator) 
: position(n*(2*m))
, velocity(n*(2*m), 0) /* Velocity initialized to 0 for all particles. */
, pBestScore(0)
, pBest(n*(2*m))
{
	generate_n(this->position.begin(), n, generator); /* Position initialized to uniformily distributed random values */
}

vector<unsigned short> getPriorityList (vector<float>& pos, unsigned short nJobs) {
	vector<float> encodedPriorities(pos.begin(), pos.begin() + nJobs);
	vector<unsigned short> decodedPriorities(nJobs);

	auto begin = encodedPriorities.begin();

	int i = 0;

	while(i < nJobs) {
		unsigned short index = distance(begin, min_element(begin, begin + encodedPriorities.size()));
		decodedPriorities[i] = index + 1;
		encodedPriorities[index] = M;
		cout << decodedPriorities[i] << " ";
		i++;
	}
	cout << endl;

	return decodedPriorities;
};


PSO::PSO(Instance& inst, PSOParams& params)
: gBestScore(0)
, gBest(inst.n*(2*inst.m))
, instance(inst)
, parameters(params)
{
	default_random_engine gen(params.seed);
	uniform_real_distribution<float> dist(params.uMin, params.uMax);
	this->generator = bind(dist, gen);
	this->swarm = vector<Particle>(params.nParticles, Particle(inst.n, inst.m, this->generator));
	getPriorityList(this->swarm[0].getPos(), inst.n);
}