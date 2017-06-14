#ifndef _H_PRIORITY_HPP_INCLUDED_
#define _H_PRIORITY_HPP_INCLUDED_

#include "base/heuristic.hpp"
#include "instance.hpp"
#include <algorithm>

using namespace std;

class HPriority : public Heuristic
{
public:
	void initialize(Instance& instance);
	void decode(Particle& particle, vector<vector<unsigned short>>& routes, vector<float>& zk);
private:
	Instance instance;
	vector<vector<float>> cij;
};

#endif