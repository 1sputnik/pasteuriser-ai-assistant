#include "Scaler.h"

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
