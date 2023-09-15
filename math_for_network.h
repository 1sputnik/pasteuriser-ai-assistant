#pragma once
#include "Scaler.h"

// математические функции
double sigm(double x);
double dif_sigm(double y);
double dif_tanh(double y);

// вектора (одномерные массивы)
void create_hollow_vector(double*& arr, int size);
void randfill_vector(double*& arr, int size, double deep);
void create_random_vector(double*& arr, int size, double deep = 1.0);
void copy_vector(double*& in_data, double*& out_data, size_t size);

double* create_input_vector(DataVector& data, int cut_border, int b);

// матрицы (двумерные массивы)
void create_hollow_matrix(double**& arr, size_t rows, size_t colls);
void randfill_matrix(double**& arr, size_t rows, size_t colls, double deep);
void fill_matrix(double**& arr, size_t rows, size_t colls, double value = 0);
void create_random_matrix(double**& arr, size_t rows, size_t colls, double deep = 1.0);
void create_matrix(double**& arr, size_t rows, size_t colls, double value = 0);
void copy_matrix(double**& in, double**& out, size_t rows, size_t colls);
void delete_matrix(double**& arr, size_t rows);
