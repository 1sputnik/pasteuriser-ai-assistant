#pragma once

#include <vector>
#include "../Common/DataFormats.h"


typedef std::pair<double, double> double_pair;


double count_average_vector_value(const vector<OCDF>& data);


class Scaler {
	// диапазон скейлинга
	double max_scale;
	double min_scale;

	// максимальный и минимальный элемент скейлинга
	double max_element;
	double min_element;

	// разница диапазонов
	double scale_range;
	double real_range;
	void count_ranges();

	// для определения 
	std::pair<double, double> find_min_max_value(const vector<OCDF>& data);
	std::pair<double, double> find_min_max_value(const vector<TDF>& data);

public:
	Scaler();
	Scaler(double min, double max, double step_from_border_percent = 0);
	Scaler(double min_element, double max_element, double min_scale, double max_scale, double step_from_border_percent = 0);

	void configure(double_pair min_max);
	void configure(double min_element, double max_element, double step_from_border_percent = 0);
	void configure(const vector<OCDF>& data, double step_from_border_percent = 0, bool do_per_average = false);
	void configure(const vector<TDF>& data, double step_from_border_percent = 0);
	std::pair<double, double> get_configuration();

	void set_scale(double_pair min_max);
	void set_scale(double min_scale, double max_scale, double step_from_border_percent = 0);
	void set_scale(const vector<OCDF>& data, double step_from_border_percent = 0, bool do_per_average = false);
	void set_scale(const vector<TDF>& data, double step_from_border_percent = 0);
	std::pair<double, double> get_scale();

	double count_scale(double value);
	void scale(double& value);
	void scale(vector<OCDF>& data);
	void scale(vector<TDF>& data);

	double count_unscale(double value);
	void unscale(double& value);
	void unscale(vector<OCDF>& data);
	void unscale(vector<TDF>& data);
};
