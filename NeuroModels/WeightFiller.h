#pragma once

#include <vector>
#include "../Common/DataFormats.h"

using std::vector;

void randfill_vector(vector<double>& data, double deep);
vector<double> create_hollow_vector(int size);
vector<double> create_random_vector(int size, double deep);

vector<OCDF> sub_vector(vector<OCDF>& data, int size, int bias);
