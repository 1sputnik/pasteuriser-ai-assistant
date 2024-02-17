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

OneCIDDataFormat find_nearest_element_before_given(vector<OneCIDDataFormat> data, long long time_border) {
	OneCIDDataFormat nearest_element = data[0];
	for (size_t i = 1; i < data.size(); i++) {
		if (data[i].time > time_border) {
			return nearest_element;
		}
		else {
			nearest_element = data[i];
		}
	}
	return OneCIDDataFormat();
}
OneCIDDataFormat find_nearest_element_after_given(vector<OneCIDDataFormat> data, long long time_border) {
	OneCIDDataFormat nearest_element = data[data.size() - 1];
	for (size_t i = data.size() - 2; i > 0; i--) {
		if (data[i].time < time_border) {
			return nearest_element;
		}
		else {
			nearest_element = data[i];
		}
	}
	return OneCIDDataFormat();
}
vector<OneCIDDataFormat> cut_data_per_time(vector<OneCIDDataFormat> data, long long time_border, bool cut_trend, bool include_time_border_to_data) {
	vector<OneCIDDataFormat> cut_data;
	cut_data = data;

	if (cut_trend) {
		while (true) {
			auto removed_element = std::find_if(cut_data.begin(), cut_data.end(), [time_border](OneCIDDataFormat datum) { return datum.time > time_border; });
			if (removed_element == cut_data.end())
				break;
			else
				cut_data.erase(removed_element);
		}
		if (include_time_border_to_data) {
			if (cut_data[cut_data.size() - 1].time == time_border) {
				return cut_data;
			}
			else if (cut_data[cut_data.size() - 1].time < time_border) {
				if (data[0].time > time_border) {
					OneCIDDataFormat nearest_element = find_nearest_element_after_given(data, time_border);
					OneCIDDataFormat temp(cut_data[cut_data.size() - 1].cid, time_border, float(time_border - cut_data[cut_data.size() - 1].time) * ((nearest_element.value - cut_data[cut_data.size() - 1].value)
						/ (nearest_element.time - cut_data[cut_data.size() - 1].time)) + cut_data[cut_data.size() - 1].value);
					cut_data.insert(cut_data.end(), temp);
				}
				else {
					cut_data.insert(cut_data.end(), cut_data[cut_data.size() - 1]);
				}
			}
		}
	}
	else {
		while (true) {
			auto removed_element = std::find_if(cut_data.begin(), cut_data.end(), [time_border](OneCIDDataFormat datum) { return datum.time < time_border; });
			if (removed_element == cut_data.end())
				break;
			else
				cut_data.erase(removed_element);
		}
		if (include_time_border_to_data) {
			if (cut_data[0].time == time_border) {
				return cut_data;
			}
			else if (cut_data[0].time > time_border) {
				if (data[0].time < time_border) {
					OneCIDDataFormat nearest_element = find_nearest_element_before_given(data, time_border);
					OneCIDDataFormat temp(cut_data[0].cid, time_border, float(time_border - nearest_element.time) * ((cut_data[0].value - nearest_element.value)
						/ (cut_data[0].time - nearest_element.time)) + nearest_element.value);
					cut_data.insert(cut_data.begin(), temp);
				}
				else {
					cut_data.insert(cut_data.begin(), cut_data[0]);
				}
			}
		}
	}

	return cut_data;
}

vector<TableDataFormat> cut_percent_data(vector<TableDataFormat> data, double cut_percent, bool cut_trend) {
	vector<TableDataFormat> cut_data;

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

vector<TableDataFormat> cut_quntity_data(vector<TableDataFormat> data, size_t cut_quantity, bool cut_trend) {
	vector<TableDataFormat> cut_data;

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

vector<OneCIDDataFormat> parsing_data_per_cid(vector<OneCIDDataFormat> data, short cid) {
	vector<OneCIDDataFormat> parsed_data;

	for (size_t i = 0; i < data.size(); i++) {
		if (data[i].cid == cid)
			parsed_data.push_back(data[i]);
	}

	if (parsed_data.size() == 0)
		return data;

	return parsed_data;
}

vector<OneCIDDataFormat> parsing_data_per_cid(vector<TableDataFormat> data, short cid) {
	vector<OneCIDDataFormat> parsed_data(data.size());

	for (size_t i = 0; i < data.size(); i++) {
		switch (cid)
		{
		case 1:
			parsed_data.at(i) = OCDF(cid, data[i].time, data[i].cid_1_value);
			break;

		case 2:
			parsed_data.at(i) = OCDF(cid, data[i].time, data[i].cid_2_value);
			break;

		case 3:
			parsed_data.at(i) = OCDF(cid, data[i].time, data[i].cid_3_value);
			break;

		case 4: 
			parsed_data.at(i) = OCDF(cid, data[i].time, data[i].cid_4_value);
			break;

		case 5:
			parsed_data.at(i) = OCDF(cid, data[i].time, data[i].cid_5_value);
			break;

		case 6:
			parsed_data.at(i) = OCDF(cid, data[i].time, data[i].cid_6_value);
			break;

		default:
			break;
		}
	}

	return parsed_data;
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
