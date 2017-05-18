#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifndef _INSTANCE_HPP_INCLUDED_
#define _INSTANCE_HPP_INCLUDED_

using namespace std;

class Instance
{
	public:
		//Instance();
		//~Instance();

		Instance* load(const char*);

		unsigned short n,m; /* Number of jobs and technicians */
		vector<unsigned short> w; /* Job priority list */
		vector<vector<bool>> s; /* Skills matrix (n x m) */
		vector<unsigned short> p; /* Jobs execution time list */
		vector<unsigned short> e; /* Jobs time window start list */
		vector<unsigned short> l; /* Jobs time window end list */
		vector<unsigned short> a; /* Technician time window start list */
		vector<unsigned short> b; /* Technician time window end list */

		unsigned short MZ, MW, M; /* Model normalizers proposed by Damm et al. (see paper) */

		vector<vector<float>> c; /* Job location in (x,y) coordinates */

	private:
		bool initialized;
};


#endif