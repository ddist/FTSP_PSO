#include "instance.hpp"

Instance* Instance::load(const char* fdir) {
	ifstream file (fdir);

	if(!file.is_open()) {
		cout << "Can't open file " << fdir << endl;
		return NULL;
	}

	file >> this->n;
	this->N = this->n + 1;
	file >> this->m;

	this->w.resize(N);
	for(int i=0;i<this->N;i++) {
		file >> this->w[i];
	}

	this->s.resize(this->N);
	for(int i=0;i<this->N;i++) {
		this->s[i].resize(this->m);
		for(int j=0;j<this->m;j++) {
			file >> this->s[i][j];
		}
	}

	this->p.resize(this->N);
	for(int i=0;i<this->N;i++) {
		file >> this->p[i];
	}

	this->e.resize(this->N);
	for(int i=0;i<this->N;i++) {
		file >> this->e[i];
	}

	this->l.resize(this->N);
	for(int i=0;i<this->N;i++) {
		file >> this->l[i];
	}

	this->a.resize(this->m);
	for(int i=0;i<this->m;i++) {
		file >> this->a[i];
	}

	this->b.resize(this->m);
	for(int i=0;i<this->m;i++) {
		file >> this->b[i];
	}

	file >> this->M;
	file >> this->MW;
	file >> this->MZ;

	this->c.resize(this->N);
	for(int i=0;i<this->N;i++) {
		this->c[i].resize(2);
		file >> this->c[i][0];
		file >> this->c[i][1];
	}

	file.close();
	return this;
}