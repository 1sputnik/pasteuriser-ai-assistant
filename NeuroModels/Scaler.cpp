#include "Scaler.h"


using std::vector;
using std::pair;
using std::make_pair;

double count_max_range_of_average(std::pair<double, double> min_max, double average); 

Scaler::Scaler() {
	this->max_scale = 1.0;
	this->min_scale = 0.0;

	this->min_element = -1.0;
	this->max_element = 1.0;

	this->count_ranges();
}

Scaler::Scaler(double min_element , double max_element, double step_from_border_percent) : Scaler() {
	this->configure(min_element, max_element, step_from_border_percent);
}

Scaler::Scaler(double min_element, double max_element, double min_scale, double max_scale, double step_from_border_percent) {
	this->set_scale(min_scale, max_scale);
	this->configure(min_element, max_element, step_from_border_percent);
}


void Scaler::configure(double_pair min_max) {
	this->configure(min_max.first, min_max.second);
}

void Scaler::configure(double min_element, double max_element, double step_from_border_percent) {
	if (min_element >= max_element) {
		throw std::invalid_argument("min element is greater than or equal max element");
	}
	this->max_element = max_element + ((max_element + min_element) / 2) * step_from_border_percent;
	this->min_element = min_element - ((max_element + min_element) / 2) * step_from_border_percent;
}

void Scaler::configure(const vector<OCDF>& data, double step_from_border_percent, bool do_per_average) {
	pair<double, double> min_max = this->find_min_max_value(data);
	if (do_per_average) {
		double average_value = count_average_vector_value(data);
		double max_range = count_max_range_of_average(min_max, average_value);
		this->configure(average_value - max_range, average_value + max_range, step_from_border_percent);
	}
	else {
		this->configure(min_max.first, min_max.second, step_from_border_percent);
	}
}

void Scaler::configure(const vector<TDF>& data, double step_from_border_percent) {
	pair<double, double> min_max = this->find_min_max_value(data);
	this->configure(min_max.first, min_max.second, step_from_border_percent);
}

pair<double, double> Scaler::get_configuration() {
	return make_pair(this->min_element, this->max_element);
}

void Scaler::set_scale(double_pair min_max) {
	this->set_scale(min_max.first, min_max.second);
}

void Scaler::set_scale(double min_scale, double max_scale, double step_from_border_percent) {
	if (min_scale >= max_scale) {
		throw std::invalid_argument("min scale is greater than or equal max scale");
	}
	this->min_scale = min_scale - ((max_scale + min_scale) / 2) * step_from_border_percent;
	this->max_scale = max_scale + ((max_scale + min_scale) / 2) * step_from_border_percent;
}

void Scaler::set_scale(const vector<OCDF>& data, double step_from_border_percent, bool do_per_average) {
	pair<double, double> min_max = this->find_min_max_value(data);
	if (do_per_average) {
		double average_value = count_average_vector_value(data);
		double max_range = count_max_range_of_average(min_max, average_value);
		this->configure(average_value - max_range, average_value + max_range, step_from_border_percent);
	}
	else {
		this->set_scale(min_max.first, min_max.second, step_from_border_percent);
	}
}

void Scaler::set_scale(const vector<TDF>& data, double step_from_border_percent) {
	pair<double, double> min_max = this->find_min_max_value(data);
	this->set_scale(min_max.first, min_max.second, step_from_border_percent);
}

pair<double, double> Scaler::get_scale() {
	return make_pair(this->min_scale, this->max_scale);
}

double Scaler::count_scale(double value) {
	count_ranges();
	return (((value - this->min_element) * this->scale_range) / this->real_range) + this->min_scale;
}

void Scaler::scale(double& value) {
	value = this->count_scale(value);
}

void Scaler::scale(vector<OCDF>& data) {
	for (int i = 0; i < data.size(); i++) {
		data[i].value = (float)this->count_scale(data[i].value);
	}
}

void  Scaler::scale(vector<TDF>& data) {
	for (int i = 0; i < data.size(); i++) {
		data[i].cid_1_value = (float)this->count_scale(data[i].cid_1_value);
		data[i].cid_2_value = (float)this->count_scale(data[i].cid_2_value);
		data[i].cid_3_value = (float)this->count_scale(data[i].cid_3_value);
		data[i].cid_4_value = (float)this->count_scale(data[i].cid_4_value);
		data[i].cid_5_value = (float)this->count_scale(data[i].cid_5_value);
		data[i].cid_6_value = (float)this->count_scale(data[i].cid_6_value);
	}
}

double Scaler::count_unscale(double value) {
	count_ranges();
	return (((value - this->min_scale) * this->real_range) / this->scale_range) + this->min_element;
}


void Scaler::unscale(double& value) {
	value = this->count_unscale(value);
}

void Scaler::unscale(vector<OCDF>& data) {
	for (int i = 0; i < data.size(); i++) {
		data[i].value = (float)this->count_unscale(data[i].value);
	}
}

void Scaler::unscale(vector<TDF>& data) {
	for (int i = 0; i < data.size(); i++) {
		data[i].cid_1_value = (float)this->count_unscale(data[i].cid_1_value);
		data[i].cid_2_value = (float)this->count_unscale(data[i].cid_2_value);
		data[i].cid_3_value = (float)this->count_unscale(data[i].cid_3_value);
		data[i].cid_4_value = (float)this->count_unscale(data[i].cid_4_value);
		data[i].cid_5_value = (float)this->count_unscale(data[i].cid_5_value);
		data[i].cid_6_value = (float)this->count_unscale(data[i].cid_6_value);
	}
}


void Scaler::count_ranges() {
	this->scale_range = this->max_scale - this->min_scale;
	this->real_range = this->max_element - this->min_element;
};

pair<double, double> Scaler::find_min_max_value(const vector<OCDF>& data) {
	double max_el = data[0].value;
	double min_el = data[0].value;
	for (int i = 0; i < data.size(); i++) {
		if (max_el < data[i].value)
			max_el = data[i].value;
		if (min_el > data[i].value)
			min_el = data[i].value;
	}
	return make_pair(min_el, max_el);
}

pair<double, double> Scaler::find_min_max_value(const vector<TDF>& data) {
	double max_el = data[0].cid_1_value;
	double min_el = data[0].cid_1_value;
	for (int i = 0; i < data.size(); i++) {
		if (max_el < data[i].cid_1_value) {
			max_el = data[i].cid_1_value;
			continue;
		}
		if (max_el < data[i].cid_2_value) {
			max_el = data[i].cid_2_value;
			continue;
		}
		if (max_el < data[i].cid_3_value) {
			max_el = data[i].cid_3_value;
			continue;
		}
		if (max_el < data[i].cid_4_value) {
			max_el = data[i].cid_4_value;
			continue;
		}
		if (max_el < data[i].cid_5_value) {
			max_el = data[i].cid_5_value;
			continue;
		}
		if (max_el < data[i].cid_6_value) {
			max_el = data[i].cid_6_value;
			continue;
		}
		if (min_el > data[i].cid_1_value) {
			min_el = data[i].cid_1_value;
			continue;
		}
		if (min_el > data[i].cid_2_value) {
			min_el = data[i].cid_2_value;
			continue;
		}
		if (min_el > data[i].cid_3_value) {
			min_el = data[i].cid_3_value;
			continue;
		}
		if (min_el > data[i].cid_4_value) {
			min_el = data[i].cid_4_value;
			continue;
		}
		if (min_el > data[i].cid_5_value) {
			min_el = data[i].cid_5_value;
			continue;
		}
		if (min_el > data[i].cid_6_value) {
			min_el = data[i].cid_6_value;
			continue;
		}
	}
	return make_pair(min_el, max_el);
}

double count_max_range_of_average(std::pair<double, double> min_max, double average) {
	if (average - min_max.first > min_max.second - average)
		return average - min_max.first;
	else
		return min_max.second - average;
}

double count_average_vector_value(const vector<OCDF>& data) {
	double average = 0;
	if (data.size() >= 2500) {
		double temp_average = 0;
		int step_averaging = 25;
		int counter_averaging = 0;
		for (size_t i = 1; step_averaging * (counter_averaging + 1) < data.size() + 1; i++) {
			if (i % step_averaging == 0) {
				average += temp_average / step_averaging;
				temp_average = 0;
				counter_averaging++;
				temp_average += data[i - 1].value;
			}
			else {
				temp_average += data[i - 1].value;
			}
		}
		average /= counter_averaging;
	}
	else {
		for (size_t i = 0; i < data.size(); i++)
			average += data[i].value;
		average /= data.size();
	}
	return average;
}
