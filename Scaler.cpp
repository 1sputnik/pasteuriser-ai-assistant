#include "Scaler.h"

void cut_vector(DataVector& in_data, DataVector& data, double cut_percent, bool cut_trend, size_t range) {
	in_data.clear();
	if (cut_trend) {
		in_data.resize(size_t(data.size() * cut_percent) + range);
		for (size_t i = 0; i < in_data.size(); i++) {
			in_data[i] = data[data.size() * (1.0 - cut_percent) - range + i];
		}
	}
	else {
		in_data.resize(size_t(data.size() * cut_percent));
		for (size_t i = 0; i < in_data.size(); i++)
			in_data[i] = data[i];
	}
}

void parsing_data_per_cid(DataVector& in_data, DataVector out_data, __int8 cid) {
	in_data.clear();
	for (int i = 0; i < out_data.size(); i++) {
		if (out_data[i].cid == cid)
			in_data.push_back(out_data[i]);
	}
}

size_t find_max_element_before_border(DataVector& data, size_t border, size_t i) {
	if (i < 0)
		i = 0;
	while (i < data.size()) {
		if (data[i].time > border) {
			i -= 1;
			break;
		}
		i = i + 1;
	}
	if (i == data.size())
		return -1;
	else
		return i;
}
void right_range(DataVector& data, int range) {
	size_t intervals = int((data[data.size() - 1].time - data[0].time) / range);
	__int8 temp_cid = data[0].cid;
	size_t time_0 = data[0].time;
	DataVector temp_data(intervals);
	for (int i = 0; i < intervals; i++) {
		temp_data[i].cid = temp_cid;
		temp_data[i].time = i * range;
	}
	size_t up = 0;
	size_t down = 0;
	for (int i = 0; i < intervals; i++) {
		down = find_max_element_before_border(data, temp_data[i].time, down);
		up = down + 1;
		if (data[down].time == temp_data[i].time) {
			temp_data[i].value = data[down].value;
			continue;
		}
		if (data[up].time == temp_data[i].time) {
			temp_data[i].value = data[up].value;
			continue;
		}
		if (down == data.size() - 1) {
			temp_data[i].value = data[down].value;
			break;
		}
		double tega = double(temp_data[i].time - data[down].time)
			/ (double(temp_data[i].time - data[down].time) + double(data[up].time - temp_data[i].time));
		double unta = 1 - tega;
		temp_data[i].value = unta * data[down].value + tega * data[up].value;
	}
	data.clear();
	data = temp_data;
}


void Scaler::count_ranges() {
	this->d = this->max - this->min;
	this->D = this->max_element - this->min_element;
};
void Scaler::configure(double max, double min) {
	this->max_element = max;
	this->min_element = min;
}
void Scaler::configure(DataVector& data) {
	this->max_element = data[0].value;
	this->min_element = data[0].value;
	for (int i = 0; i < data.size(); i++) {
		if (this->max_element < data[i].value)
			this->max_element = data[i].value;
		if (this->min_element > data[i].value)
			this->min_element = data[i].value;
	}
}
void Scaler::configure(double* data, size_t size) {
	this->max_element = data[0];
	this->min_element = data[0];
	for (int i = 0; i < size; i++) {
		if (this->max_element < data[i])
			this->max_element = data[i];
		if (this->min_element > data[i])
			this->min_element = data[i];
	}
}
void Scaler::set_scale(double max, double min) {
	this->max = max;
	this->min = min;
}
void Scaler::scale(double& value) {
	count_ranges();
	value = ((((value)-this->min_element) * this->d) / this->D) + this->min;
}
void Scaler::scale(DataVector& data) {
	count_ranges();
	for (int i = 0; i < data.size(); i++) {
		data[i].value = ((((data[i].value) - this->min_element) * this->d) / this->D) + this->min;
	}
}
void Scaler::scale(double*& data, size_t size) {
	count_ranges();
	for (int i = 0; i < size; i++) {
		data[i] = ((((data[i]) - this->min_element) * this->d) / this->D) + this->min;
	}
}
void Scaler::unscale(double& value) {
	count_ranges();
	value = ((((value)-this->min) * this->D) / this->d) + this->min_element;
}
void Scaler::unscale(DataVector& data) {
	count_ranges();
	for (int i = 0; i < data.size(); i++) {
		data[i].value = ((((data[i].value) - this->min) * this->D) / this->d) + this->min_element;
	}
}
void Scaler::unscale(double*& data, size_t size) {
	count_ranges();
	for (int i = 0; i < size; i++) {
		data[i] = ((((data[i]) - this->min) * this->D) / this->d) + this->min_element;
	}
}
Scaler::Scaler() {
	this->max = 1.0;
	this->min = double();
}
Scaler::Scaler(double max, double min) {
	this->max = max;
	this->min = min;
}
