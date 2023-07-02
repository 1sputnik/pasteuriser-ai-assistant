#pragma once
#include "data_changer.h"

typedef unsigned __int16 unint16;

class RecurrentNeuron {
protected:
	// ��������� ����
	unint16 epochs; // ���������� ���� ����
	unint16 input_range; // ������ ������� ������� � ����
	unint16 hidden_range; // ������ ������� ������� � ����
	unint16 output_range; // ������ �������� ������� � ����
	double rate; // �������� �������� ����
	double precision = 0.0000001; // �������� ������������
	double e_predict = 0; // ������ ������������

	// ����������, �������������� ������ ����
	double** h; // ������������� ������

	// ���� 
	double** W_y; // ���� ��� ������������

public:
	virtual void fit(DataVector& train_data) = 0; // ������� ����
	virtual void predict(DataVector& test_data) = 0; // ��������� ������������

	RecurrentNeuron(double rate, unint16 epochs, unint16 input_range);
};

class LSTM;

void load_model(LSTM& lstm, string file_name);
void dump_model(LSTM& lstm, string file_name);

class LSTM : public RecurrentNeuron{
	// ����������, �������������� ������ ����
	double** C; // ������������ ������ 
	double* last_C_from_train = nullptr; // ��������� ������ ������������ ������ ��� �������� 
	double* last_h_from_train = nullptr; // ��������� ������ ������������� ������ ��� �������� 

	double* de_dF_futur; // ��������������� ������ �� ������ ��������� ��������� �������� ����
	double* de_dO_futur; // ��������������� ������ �� ������ ������ ��������� �������� ����
	double* de_dG_futur; // ��������������� ������ �� ������ ��������� ��������� �������� ����
	double* de_dI_futur; // ��������������� ������ �� ������ ����� ��������� ��������
	double* de_dC_futur; // ��������������� ������ �� ������������ ������ ��������� ��������
	double* forgate_futur; // ����� ��������� ��������� �������� 

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

	// ������� ����������� �������������� ������� ��� ������ ����
	void train(double* x, double* y_real, size_t k = 0);
	double* forecast(double* x, size_t k = 0);

	// ������� ��� ���������� � �������� ����
	friend void load_model(LSTM& lstm, string file_name);
	friend void dump_model(LSTM& lstm, string file_name);

public:
	// �������, ����������� ������ ������ � �������������� ������� ���� ��� ����������� �������� ��� ��������
	virtual void fit(DataVector& train_data) override;
	virtual void predict(DataVector& test_data) override;

	LSTM(double rate, unint16 epochs, unint16 input_range, unint16 hidden_range, unint16 output_range = 1);
	~LSTM();
};
