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
				if(norm1(pos[3*i], pos[3*i+1], inst.c[j+1][0], inst.c[j+1][1]) <= abs(pos[3*i+2])) {
					coverage[i][j] = inst.w[j+1];
				}
			}
		}
	}
}

void getDistanceMatrix(Instance& inst, vector<vector<float>>& distances) {
	for(int i=0;i<inst.N;i++) {
		for (int j=0;j<inst.N;j++) {
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

void decode(Particle& particle, Instance& instance, vector<vector<float>>& cij, vector<vector<unsigned short>>& routes, vector<float>& zk) {
	auto coverageMatrix = vector<vector<unsigned short>>(instance.m, vector<unsigned short>(instance.n));
	getCoverageMatrix(particle.position, instance, coverageMatrix);

	auto tk = vector<float>(instance.m, 0.0);
	auto done = vector<bool>(instance.m, false);
	auto jobStatus = vector<unsigned short>(instance.w);
	unsigned short mDone = 0, nFinished = 0;

	bool impossible = false, fixing = false;

	do {
		for(int i=0; i<instance.m; i++) {
			// Skip technicians with complete route
			if(done[i]) continue;
			// Set tk to the begining of technician window at first iteration
			if(tk[i]==0.0) tk[i] = (float)instance.a[i];
			unsigned short best = 0, bestIndex = 0;
			unsigned short prevJob = routes[i].at(routes[i].size()-2);
			// Check constrains and select higher priority job
			for(int j=0;j<instance.n;j++) {
				if(coverageMatrix[i][j] == 0 || jobStatus[j+1] == 0) continue;
				if((float)instance.l[j+1] < tk[i] + cij[prevJob][j+1] + (float)instance.p[j+1]) {
					coverageMatrix[i][j] = 0;
					continue;
				}
				if(max(tk[i] + cij[prevJob][j+1], (float)instance.e[j+1]) > (float)instance.b[i] - cij[j+1][0] - (float)instance.p[j+1]) {
					coverageMatrix[i][j] = 0;
					continue;
				}
				if(coverageMatrix[i][j] > best) {
					best = coverageMatrix[i][j];
					bestIndex = (unsigned short)j;
				}
			}
			// If no job available, set route as complete or fix it if technician hasn't left origin
			if(best==0) {
				if(routes[i].size() > 2) {
					done[i] = true;
					mDone++;
				} else {
					// If the radius constrain was already discarded and no available job was found, 
					// then its impossible to satisfy the "all technician must leave origin" constrain.
					if(fixing) {
						impossible = true;
						break;
					}
					// Make all jobs that the technician has the skills to execute, available if its
					// not already in some other technician route. This bypasses the coverage radius
					// in order to increase the available jobs for the technician
					for(int k=0;k<instance.n;k++) {
						coverageMatrix[i][k] = jobStatus[k+1]*instance.s[k+1][i];
					}
					fixing = true;
					// Decrement i to repeat the constrain check for this technician (better than a goto??)
					i--;
				}
				continue;
			}
			// Add selected job to route
			routes[i].insert(routes[i].end()-1, bestIndex+1);
			// Set tk and zk and jobStatus
			tk[i] = max(tk[i] + cij[prevJob][bestIndex+1], (float)instance.e[bestIndex+1]) + (float)instance.p[bestIndex+1];
			zk[i] = (float)instance.b[i] - tk[i] - cij[bestIndex+1][0];
			jobStatus[bestIndex+1] = 0;
			nFinished++;
			fixing = false;
		}
		// Exit if its imposible to satisfy some constrain
		if(impossible) break;
	} while(mDone<instance.m);
	// If there are unassigned jobs after building the routes, check if they can be added to any route without breaking constrains.
	if(mDone==instance.m && nFinished != instance.n) {
		unsigned short fixed = 0;
		for(int j = 0; j < instance.n - nFinished; j++) {
			int job = distance(jobStatus.begin(), max_element(jobStatus.begin(), jobStatus.end()));
			for(int i = 0; i < instance.m; i++) {
				// Check constrains
				if(instance.s[job][i] == 0 || job == 0) continue;
				unsigned short prevJob = routes[i].at(routes[i].size()-2);
				if((float)instance.l[job] < tk[i] + cij[prevJob][job] + (float)instance.p[job]) continue;
				if(max(tk[i] + cij[prevJob][job], (float)instance.e[job]) > (float)instance.b[i] - cij[job][0] - (float)instance.p[job]) continue;
				// Add job to route
				routes[i].insert(routes[i].end()-1, job);
				// Set tk and zk
				tk[i] = max(tk[i] + cij[prevJob][job], (float)instance.e[job]) + (float)instance.p[job];
				zk[i] = (float)instance.b[i] - tk[i] - cij[job][0];
				fixed++;
				//cout << job << "-";
				break;
			}
			jobStatus[job] = 0;
		}
		//cout << " ";
	}
}

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

	auto distances = vector<vector<float>>(this->instance.N, vector<float>(this->instance.N, 0.0));
	getDistanceMatrix(this->instance, distances);
	unsigned short nIter = 0;

	uniform_real_distribution<float> dist(0.0,1.0);
	function<float()> gen = bind(dist, this->generator);

	do 
	{
		for(int i=0; i < this->parameters.nParticles; i++) {
			// Decode particle position into technician routes
			auto routes = vector<vector<unsigned short>>(this->instance.m, vector<unsigned short>(2, 0));
			auto zk = vector<float>(this->instance.m, 0.0);
			
			decode(this->swarm[i], this->instance, distances, routes, zk);
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