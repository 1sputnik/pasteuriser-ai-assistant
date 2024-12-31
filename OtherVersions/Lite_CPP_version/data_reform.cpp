#include "data_reform.h"

void cut_vector(DataVector& output_data, DataVector& data, double cut_percent, bool cut_trend, size_t range) {
	output_data.clear();
	if (cut_trend) {
		output_data.resize(size_t(data.get_size() * cut_percent) + range);
		for (size_t i = 0; i < output_data.get_size(); i++)
			output_data[i] = data[data.get_size() * (1.0 - cut_percent) - range + i];
	}
	else {
		output_data.resize(size_t(data.get_size() * cut_percent));
		for (size_t i = 0; i < output_data.get_size(); i++)
			output_data[i] = data[i];
	}
}

size_t find_max_element_before_border(DataVector& data, size_t border, size_t i) {
	if (i < 0)
		i = 0;
	while (i < data.get_size()) {
		if (data[i].time > border) {
			i -= 1;
			break;
		}
		i = i + 1;
	}
	if (i == data.get_size())
		return -1;
	else
		return i;
}

void fix_time_range(DataVector& data, int range) {
	size_t intervals = int((data[data.get_size() - 1].time - data[0].time) / range);
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
		if (down == data.get_size() - 1) {
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

void parsing_data_per_cid(DataVector& output_data, DataVector input_data, __int8 cid) {
	output_data.clear();
	for (int i = 0; i < input_data.get_size(); i++) {
		if (input_data[i].cid == cid)
			output_data.push_back(input_data[i]);
	}
}
