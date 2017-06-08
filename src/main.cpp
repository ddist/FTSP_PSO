#include "instance.hpp"
#include "pso.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
	Instance inst = Instance();
	inst.load(argv[1]);
	PSOParams params;
	params.seed = 1564;
	params.uMax = 100.0;
	params.uMin = 0.0;
	params.vMin = 0.0;
	params.vMax = 100.0;
	params.q1 = 0.6;
	params.q2 = 0.4;
	params.nParticles = 10;
	PSO optimizer(inst, params);
	return 1;
}