#include "math_for_network.h"

// математические функции
double sigm(double x) {
	return 1.0 / (1.0 + exp(-x));
}

double dif_sigm(double y) {
	return y * (1.0 - y);
}

double dif_tanh(double y) {
	return 1.0 - y * y;
}

// вектора (одномерные массивы)
void randfill_vector(double*& arr, int size, double deep) {
	// deep - сколько нулей будет в значении весов: value = 0.<deep><[0, rank - 1]>
	double rank = 1000; // сколько знаков нам нужно при рандоме: [0, rank - 1]
	for (size_t i = 0; i < size; i++)
		arr[i] = double((rand() % (2 * int(rank) + 1)) - int(rank)) / (deep * rank);
}

double* create_random_vector(int size, double deep) {
	double* arr = new double[size];
	randfill_vector(arr, size, deep);
	return arr;
}

void copy_vector(double*& output_data, double*& input_data, size_t size) {
	delete[] output_data;
	output_data = new double[size];
	for (int i = 0; i < size; i++)
		output_data[i] = input_data[i];
}

double* create_input_vector(DataVector& data, int cut_border, int bias) {
	double* input_vector = new double[cut_border];
	for (int i = 0; i < cut_border; i++)
		input_vector[i] = data[i + bias].value;
	return input_vector;
};

// матрицы (двумерные массивы)
void create_empty_matrix(double**& arr, size_t rows, size_t colls) {
	arr = new double* [rows];
	for (size_t i = 0; i < rows; i++)
		arr[i] = new double[colls];
}

void randfill_matrix(double**& arr, size_t rows, size_t colls, double deep) {
	// deep - сколько нулей будет в значении весов: value = 0.<deep><[0, rank -1]>
	double rank = 1000; // сколько знаков нам нужно при рандоме: [0, rank - 1]
	for (size_t i = 0; i < rows; i++)
		randfill_vector(arr[i], colls, deep);
}

void fill_matrix(double**& arr, size_t rows, size_t colls, double value) {
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < colls; j++)
			arr[i][j] = value;
}

void create_random_matrix(double**& arr, size_t rows, size_t colls, double deep) {
	create_empty_matrix(arr, rows, colls);
	randfill_matrix(arr, rows, colls, deep);
}

void create_matrix(double**& arr, size_t rows, size_t colls, double value) {
	create_empty_matrix(arr, rows, colls);
	fill_matrix(arr, rows, colls, value);
}

void copy_matrix(double**& in, double**& out, size_t rows, size_t colls) {
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < colls; j++)
			in[i][j] = out[i][j];
}

void delete_matrix(double**& arr, size_t rows) {
	for (size_t i = 0; i < rows; i++)
		delete[] arr[i];
	delete[] arr;
}
