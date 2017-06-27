#include "pso.hpp"
#include <iterator>
#include <string>
#include <cstring>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char const *argv[]) {
	PSOParams params;
	Instance inst = Instance();
	Heuristic *h;
	Topology** t;
	int n = 0;
	int inst_type = 0;
	float cg, cn, cl;
	float uq = 0.0;

	for(int i=1; i < argc;i++) {
		if(strcmp(argv[i], "-seed") == 0) {params.seed = (unsigned int)atoi(argv[i+1]); i++; continue;};
		if(strcmp(argv[i], "-inst") == 0) {inst.load(argv[i+1]); i++; continue;};
		if(strcmp(argv[i], "-type") == 0) {
			if(strcmp(argv[i+1], "RAD") == 0) {inst_type = 1;}; 
			i++; continue;
		}
		if(strcmp(argv[i], "-uMax") == 0) {
			if(inst_type) {
				params.uMax = strtof(argv[i+1], NULL)/2.0;
			} else {
				params.uMax = strtof(argv[i+1], NULL);
			}
			i++; continue;
		}
		if(strcmp(argv[i], "-uMin") == 0) {params.uMin = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-vMin") == 0) {params.vMin = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-vMax") == 0) {params.vMax = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-wMin") == 0) {params.wMin = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-wMax") == 0) {params.wMax = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-cp") == 0) {params.q = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-cg") == 0) {cg = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-cn") == 0) {cn = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-cl") == 0) {cl = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-uq") == 0) {uq = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-nParticles") == 0) {params.nParticles = (unsigned short)atoi(argv[i+1]); i++; continue;};
		if(strcmp(argv[i], "-maxIter") == 0) {params.maxIter = (unsigned short)atoi(argv[i+1]); i++; continue;};
		if(strcmp(argv[i], "-heuristic") == 0) {
			if(atoi(argv[i+1]) == 1) {
				h = new Priority();
			} else {
				h = new Urgency();
			}
			i++; 
			continue;
		}
		if(strcmp(argv[i], "-topologies") == 0) {
			string s (argv[i+1]);
			n = (int)s.size();
			t = new Topology*[n];
			for(int j=0; j < n;j++) {
				if(argv[i+1][j] == '1') {
					t[j] = new Global();
					continue;
				}
				if(argv[i+1][j] == '2') {
					t[j] = new Ring();
					continue;
				}
				if(argv[i+1][j] == '3') {
					t[j] = new FDR();
					continue;
				}
			}
			i++; continue;
		}
	}

	if(inst_type == 1) {params.uMin = -params.uMax;};

	if(uq != 0.0) {((Urgency*)h)->q = uq;};

	for(int j=0; j < n;j++) {
		if(strcmp(t[j]->name, "Global") == 0) {t[j]->q = cg; continue;};
		if(strcmp(t[j]->name, "Ring") == 0) {t[j]->q = cl; continue;};
		if(strcmp(t[j]->name, "FDR") == 0) {t[j]->q = cn; continue;};
	}

	cout << "-> Optimizing... " << std::flush;

	float best;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	PSO optimizer(inst, params, h, n, t);
	best = optimizer.solve();

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto runtime = duration_cast<milliseconds>(t2-t1).count();

	cout << "-> Best Score = " << best << endl;
	cout << "-> Runtime = " << runtime << endl;

	return 0;
}