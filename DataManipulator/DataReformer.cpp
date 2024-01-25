#include "DataReformer.h"

vector<OneCIDDataFormat> cut_percent_data(vector<OneCIDDataFormat> data, double cut_percent, bool cut_trend) {
	vector<OneCIDDataFormat> cut_data;

	if (cut_trend) {
		cut_data.resize(size_t(data.size() * cut_percent));
		for (size_t i = 0; i < cut_data.size(); i++) {
			cut_data[i] = data[data.size() * (1.0 - cut_percent) + i];
		}
	}
	else {
		cut_data.resize(size_t(data.size() * cut_percent));
		for (size_t i = 0; i < cut_data.size(); i++) {
			cut_data[i] = data[i];
		}
	}

	return cut_data;
}

vector<OneCIDDataFormat> cut_quntity_data(vector<OneCIDDataFormat> data, size_t cut_quantity, bool cut_trend) {
	vector<OneCIDDataFormat> cut_data;

	if (cut_quantity > data.size()) {
		cut_quantity = data.size();
	}
	cut_data.resize(cut_quantity);

	if (cut_trend) {
		for (size_t i = 0; i < cut_data.size(); i++) {
			cut_data[i] = data[data.size() - cut_quantity + i];
		}
	}
	else {
		for (size_t i = 0; i < cut_data.size(); i++) {
			cut_data[i] = data[i];
		}
	}

	return cut_data;
}

long long find_max_element_before_border(vector<OneCIDDataFormat>& data, size_t border, long long i) {
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
vector<OneCIDDataFormat> right_range(vector<OneCIDDataFormat> data, size_t range) {
	size_t intervals = size_t((data[data.size() - 1].time - data[0].time) / range);
	vector<OneCIDDataFormat> right_data;
	right_data.resize(intervals);

	size_t time_0 = data[0].time;
	for (int i = 0; i < intervals; i++) {
		right_data[i].cid = data[0].cid;
		right_data[i].time = time_0 + i * range;
	}

	long long up = 0;
	long long down = 0;
	for (int i = 0; i < intervals; i++) {
		down = find_max_element_before_border(data, time_0 + i * range, down);
		up = down + 1;
		if (data[down].time == right_data[i].time) {
			right_data[i].value = data[down].value;
			continue;
		}
		if (data[up].time == right_data[i].time) {
			right_data[i].value = data[up].value;
			continue;
		}
		if (down == data.size() - 1) {
			right_data[i].value = data[down].value;
			break;
		}
		else {
			right_data[i].value = ((time_0 + i * range) - data[down].time) * ((data[up].value - data[down].value) / (data[up].time - data[down].time)) + data[down].value;
			continue;
		}
	}
	return right_data;
}