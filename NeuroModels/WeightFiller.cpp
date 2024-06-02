#include "WeightFiller.h"

void randfill_vector(vector<double>& data, double deep) {
	double rank = 1000; 
	for (size_t i = 0; i < data.size(); i++)
		data[i] = double((rand() % (2 * int(rank) + 1)) - int(1.0 * rank)) / (deep * double(rank));
}

vector<double> create_hollow_vector(int size) {
	vector<double> data(size, 0.0);
	return data;
}

vector<double> create_random_vector(int size, double deep) {
	vector<double> data(size);
	randfill_vector(data, deep);
	return data;
}

vector<OCDF> sub_vector(vector<OCDF>& data, int size, int bias) {
	vector<OCDF> subvector(size);
	for (size_t i = 0; i < size; i++) {
		subvector[i] = data[i + bias];
	}
	return subvector;
}
