#pragma once
#include "data_reform.h"

class Scaler {
	// �������� ���������
	double max;
	double min;

	// ������������ � ����������� ������� ���������
	double max_element;
	double min_element;

	// ������� ����������
	double d;
	double D;
	void count_ranges();

public:
	Scaler();
	Scaler(double max, double min = 1);

	void configure(double max, double min);
	void configure(DataVector& data);
	void configure(double* data, size_t size);

	void set_scale(double max, double min);

	void scale(double& value);
	void scale(DataVector& data);
	void scale(double*& data, size_t size);
	void unscale(double& value);
	void unscale(DataVector& data);
	void unscale(double*& data, size_t size);
};