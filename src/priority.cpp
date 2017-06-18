#include "priority.hpp"

/* 2D Euclidian distance */
float norm1(float x1, float x2, float y1, float y2) {
	return sqrt( pow(x1 - y1, 2) + pow(x2 - y2, 2));
}

void getCoverageMatrix(vector<float>& pos, Instance& instance, vector<vector<unsigned short>>& coverage) {
	for(int i=0;i<instance.m;i++) {
		for(int j=0;j<instance.n;j++) {
			// Check if technician has the required skill
			if(instance.s[j+1][i]) {
				// Check if job is in technician range (euclidian distance)
				if(norm1(pos[3*i], pos[3*i+1], instance.c[j+1][0], instance.c[j+1][1]) <= abs(pos[3*i+2])) {
					coverage[i][j] = instance.w[j+1];
				}
			}
		}
	}
}

void getDistanceMatrix(Instance& instance, vector<vector<float>>& distances) {
	for(int i=0;i<instance.N;i++) {
		for (int j=0;j<instance.N;j++) {
			if(i==j) {
				distances[i][j] = 0.0;
			} else if (i>j) {
				distances[i][j] = distances[j][i];
			} else {
				distances[i][j] = norm1(instance.c[i][0], instance.c[i][1], instance.c[j][0], instance.c[j][1]);
			}
		}
	}
}

void Priority::initialize(Instance& instance) {
	this->instance = instance;
	this->cij = vector<vector<float>>(instance.N, vector<float>(instance.N, 0.0));
	getDistanceMatrix(instance, this->cij);
}


void Priority::decode(Particle& particle, vector<vector<unsigned short>>& routes, vector<float>& zk) {

	auto coverageMatrix = vector<vector<unsigned short>>(this->instance.m, vector<unsigned short>(this->instance.n));
	getCoverageMatrix(particle.position, this->instance, coverageMatrix);

	auto tk = vector<float>(this->instance.m, 0.0);
	auto done = vector<bool>(this->instance.m, false);
	auto jobStatus = vector<unsigned short>(this->instance.w);
	unsigned short mDone = 0, nFinished = 0;

	bool impossible = false, fixing = false;

	do {
		for(int i=0; i<this->instance.m; i++) {
			// Skip technicians with complete route
			if(done[i]) continue;
			// Set tk to the begining of technician window at first iteration
			if(tk[i]==0.0) tk[i] = (float)this->instance.a[i];
			unsigned short best = 0, bestIndex = 0;
			unsigned short prevJob = routes[i].at(routes[i].size()-2);
			// Check constrains and select higher priority job
			for(int j=0;j<this->instance.n;j++) {
				if(coverageMatrix[i][j] == 0 || jobStatus[j+1] == 0) continue;
				if((float)this->instance.l[j+1] < tk[i] + cij[prevJob][j+1] + (float)this->instance.p[j+1]) {
					coverageMatrix[i][j] = 0;
					continue;
				}
				if(max(tk[i] + cij[prevJob][j+1], (float)this->instance.e[j+1]) > (float)this->instance.b[i] - cij[j+1][0] - (float)this->instance.p[j+1]) {
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
					for(int k=0;k<this->instance.n;k++) {
						coverageMatrix[i][k] = jobStatus[k+1]*this->instance.s[k+1][i];
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
			tk[i] = max(tk[i] + cij[prevJob][bestIndex+1], (float)this->instance.e[bestIndex+1]) + (float)this->instance.p[bestIndex+1];
			zk[i] = (float)this->instance.b[i] - tk[i] - cij[bestIndex+1][0];
			jobStatus[bestIndex+1] = 0;
			nFinished++;
			fixing = false;
		}
		// Exit if its imposible to satisfy some constrain
		if(impossible) break;
	} while(mDone<this->instance.m);
	// If there are unassigned jobs after building the routes, check if they can be added to any route without breaking constrains.
	if(mDone==this->instance.m && nFinished != this->instance.n) {
		unsigned short fixed = 0;
		for(int j = 0; j < this->instance.n - nFinished; j++) {
			int job = distance(jobStatus.begin(), max_element(jobStatus.begin(), jobStatus.end()));
			for(int i = 0; i < this->instance.m; i++) {
				// Check constrains
				if(this->instance.s[job][i] == 0 || job == 0) continue;
				unsigned short prevJob = routes[i].at(routes[i].size()-2);
				if((float)this->instance.l[job] < tk[i] + cij[prevJob][job] + (float)this->instance.p[job]) continue;
				if(max(tk[i] + cij[prevJob][job], (float)this->instance.e[job]) > (float)this->instance.b[i] - cij[job][0] - (float)this->instance.p[job]) continue;
				// Add job to route
				routes[i].insert(routes[i].end()-1, job);
				// Set tk and zk
				tk[i] = max(tk[i] + cij[prevJob][job], (float)this->instance.e[job]) + (float)this->instance.p[job];
				zk[i] = (float)this->instance.b[i] - tk[i] - cij[job][0];
				fixed++;
				break;
			}
			jobStatus[job] = 0;
		}
	}
}