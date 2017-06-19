#include "pso.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
	Instance inst = Instance();
	inst.load(argv[1]);
	PSOParams params;
	params.seed = 1;
	params.uMax = 100.0;
	params.uMin = 0.0;
	params.vMin = -30.0;
	params.vMax = 30.0;
	params.vWeight = 1.0;
	params.q = 0.4;
	params.nParticles = 30;
	params.maxIter = 300;

	Heuristic *h;
	h = new Priority();

	Topology** t = new Topology*[2];
	t[0] = new Global(0.5);
	t[1] = new Ring(0.6, 1);

	PSO optimizer(inst, params, h, 2, t);
	optimizer.solve();
	return 0;
}