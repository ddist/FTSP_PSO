#include "instance.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
	Instance inst = Instance();
	inst.load(argv[1]);
	//inst.print();
	return 1;
}