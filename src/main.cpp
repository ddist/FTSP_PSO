#include "instance.hpp"
#include "pso.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
	Instance inst = Instance();
	inst.load(argv[1]);
	PSOParams params;
	params.seed = 1564;
	params.uMax = 1.0;
	params.uMin = 0.0;
	params.nParticles = 10;
	PSO optimizer(inst, params);
	return 1;
}