#include "instance.hpp"
#include "pso.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
	Instance inst = Instance();
	inst.load(argv[1]);
	PSOParams params;
	params.seed = 1;
	params.uMax = 100.0;
	params.uMin = 0.0;
	params.vMin = 0.0;
	params.vMax = 20.0;
	params.vWeight = 1.0;
	params.q1 = 0.2;
	params.q2 = 0.8;
	params.nParticles = 30;
	params.maxIter = 100;
	PSO optimizer(inst, params);
	optimizer.solve();
	return 1;
}