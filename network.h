#pragma once

#define ErrorInfo

#include "math_for_network.h"

typedef unsigned __int16 unint16;

class RecurrentNeuron {
protected:
	// ��������� ����
	unint16 epochs; // ���������� ���� ����
	unint16 input_range; // ������ ������� ������� � ����
	unint16 hidden_range; // ������ ������� ������� � ����
	unint16 output_range; // ������ �������� ������� � ����
	double learning_rate; 
	double target_error = 0.000001; // �������� ������������
	double predict_error = 0; // ������ ������������

	// ����������, �������������� ������ ����
	double** h; // ������������� ������

	// ���� 
	double** W_y; // ���� ��� ������������

public:
	virtual void fit(DataVector& train_data, long long batch_size = 0) = 0; // ������� ����
	virtual void predict(DataVector& test_data) = 0; // ��������� ������������

	RecurrentNeuron(double learning_rate, unint16 epochs, unint16 input_range);
};

class LSTM;

void load_model(LSTM& lstm, string file_name);
void dump_model(LSTM& lstm, string file_name);

class LSTM : public RecurrentNeuron{
	// ����������, �������������� ������ ����
	double** C; // ������������ ������ 
	double* last_C_from_train = nullptr; // ��������� ������ ������������ ������ ��� �������� 
	double* last_h_from_train = nullptr; // ��������� ������ ������������� ������ ��� �������� 

	double* de_dF_future; // ��������������� ������ �� ������ ��������� ��������� �������� ����
	double* de_dO_future; // ��������������� ������ �� ������ ������ ��������� �������� ����
	double* de_dG_future; // ��������������� ������ �� ������ ��������� ��������� �������� ����
	double* de_dI_future; // ��������������� ������ �� ������ ����� ��������� ��������
	double* de_dC_future; // ��������������� ������ �� ������������ ������ ��������� ��������
	double* forgate_future; // ����� ��������� ��������� �������� 

	// ���� 
	double** W_f; // ������� ����� ���� ��������� ��� ������� ��������
	double** W_i; // ������� ����� ���� ����� ��� ������� ��������
	double** W_g; // ������� ����� ���� ���������� ��� ������� ��������
	double** W_o; // ������� ����� ���� ������ ��� ������� ��������

	double** U_f; // ������� ����� ���� ��������� ��� ������������� ������
	double** U_i; // ������� ����� ���� ����� ��� ������������� ������
	double** U_g; // ������� ����� ���� ��������� ��� ������������� ������
	double** U_o; // ������� ����� ���� ������ ��� ������������� ������

	// ������������� ���� ���������� ��� ������� ������ ��������� ��������������� 
	double** _W_f = nullptr; 
	double** _W_i = nullptr;
	double** _W_g = nullptr;
	double** _W_o = nullptr;

	double** _U_f = nullptr;
	double** _U_i = nullptr;
	double** _U_g = nullptr;
	double** _U_o = nullptr;

	double** _W_y = nullptr;

	// ������� ��� ������ � �������������� ������
	void select_memory_for_temp_weight();
	void copy_weight();
	void free_temp_weigth();
	void clear_futur();

	// ������� ����������� �������������� ������� ��� ������ ����
	void train(double* x, double* y_real, size_t k = 0);
	double* calculate_output(const size_t k);
	double calculate_error(double* y_predict, double* y_real);
	void calculate_gates(double*& forgate_gate, double*& x, double*& input_gate, double*& output_gate, double*& state_gate, const size_t k);

	double* forecast(double* x, size_t k = 0);

	// ������� ��� ���������� � �������� ����
	friend void load_model(LSTM& lstm, string file_name);
	friend void dump_model(LSTM& lstm, string file_name);

public:
	// �������, ����������� ������ ������ � �������������� ������� ���� ��� ����������� �������� ��� ��������
	virtual void fit(DataVector& train_data, long long batch_size = 0) override;
	virtual void predict(DataVector& test_data) override;

	LSTM(double learning_rate, unint16 epochs, unint16 input_range, unint16 hidden_range, unint16 output_range);
	~LSTM();
};
