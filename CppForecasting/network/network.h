#pragma once
#include "data_changer.h"

typedef unsigned __int8 unint8;
typedef unsigned __int16 unint16;

class Neuron {
protected:
	unint8 epochs;
	unint16 input_range;
	double rate;

public:
	virtual void fit(DataVector& train_data) = 0;
	virtual void predict(DataVector& test_data, size_t add_predict_range) = 0;

	Neuron(double rate = 0.001, unint8 epochs = 1, unint16 input_range = 1);
};

class LSTM : public Neuron{
	// параметры дл€ сети 
	double precision;

	// параметрые, представл€ющие пам€ть сети
	double** h;
	double* h0_for_predict;
	double** C;
	double* C0_for_predict;
	double* de_dF_futur;
	double* de_dO_futur;
	double* de_dG_futur;
	double* de_dI_futur;
	double* de_dC_futur;
	double* forgate_futur;

	// веса 
	double** W_f;
	double** W_i;
	double** W_g;
	double** W_o;

	double** U_f;
	double** U_i;
	double** U_g;
	double** U_o;

	double* W_y;

	// смещени€
	double* b_f;
	double* b_o;
	double* b_i;
	double* b_g;

	// приватные функции
	double train(double* x, double y_real, size_t k = 0);
	double forecast(double* x, size_t k = 0);

public:
	virtual void fit(DataVector& train_data) override;
	virtual void predict(DataVector& test_data, size_t add_predict_range = 0) override;

	LSTM(double rate = 0.001, unint8 epochs = 1, unint16 input_range = 1, double precision = 0);
	~LSTM();
};
