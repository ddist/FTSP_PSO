#include "pso.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
	Instance inst = Instance();
	inst.load(argv[1]);
	PSOParams params;
	params.seed = 1;
	params.uMax = 100.0;
	params.uMin = 0.0;
	params.vMax = params.uMax/4;
	params.vMin = -params.vMax;
	params.wMax = 0.9;
	params.wMin = 0.4;
	params.q = 0.5;
	params.nParticles = 30;
	params.maxIter = 1000;

	Heuristic *h;
	h = new Urgency();

	Topology** t = new Topology*[3];
	t[0] = new Global(0.5);
	t[1] = new Ring(1.5);
	t[2] = new FDR(1.5);


	PSO optimizer(inst, params, h, 3, t);
	optimizer.solve();
	return 0;
}