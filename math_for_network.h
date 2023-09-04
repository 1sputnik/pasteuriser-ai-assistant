#pragma once
#include "Scaler.h"

// математические функции
inline double sigm(double x);
inline double dif_sigm(double y);
inline double dif_tanh(double y);

// вектора (одномерные массивы)
inline void create_hollow_vector(double*& arr, int size);
inline void randfill_vector(double*& arr, int size, double tiff);
inline void create_random_vector(double*& arr, int size, double tiff = 1.0);
inline void copy_vector(double*& in_data, double*& out_data, size_t size);

inline double* create_input_vector(DataVector& data, int cut_border, int b);

// матрицы (двумерные массивы)
inline void create_hollow_matrix(double**& arr, size_t rows, size_t colls);
inline void randfill_matrix(double**& arr, size_t rows, size_t colls, double tiff);
inline void fill_matrix(double**& arr, size_t rows, size_t colls, double value = 0);
inline void create_random_matrix(double**& arr, size_t rows, size_t colls, double tiff = 1.0);
inline void create_matrix(double**& arr, size_t rows, size_t colls, double value = 0);
inline void copy_matrix(double**& in, double**& out, size_t rows, size_t colls);
inline void delete_matrix(double**& arr, size_t rows);
