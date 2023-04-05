#pragma once
#include "data_changer.h"

typedef unsigned __int8 unint8;
typedef unsigned __int16 unint16;

class Network {
protected:
	unint8 epochs;
	unint16 input_range;
	double rate;

public:
	virtual void fit(DataVector& train_data) = 0;
	virtual void predict(DataVector& test_data, size_t add_predict_range) = 0;

	Network(double rate = 0.001, unint8 epochs = 1, unint16 input_range = 1);
};

class LSTM : public Network{
	// параметры дл€ сети 
	unint8 fit_step;
	double precision;

	// параметрые, представл€ющие пам€ть предыдущей итерации сети
	double* h;
	double* C;

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

	// приватные функции
	double train(double* x, double y_real);
	double forecast(double* x, double y_predict);

public:
	virtual void fit(DataVector& train_data) override;
	virtual void predict(DataVector& test_data, size_t add_predict_range = 0) override;

	LSTM(double rate = 0.001, unint8 epochs = 1, unint16 input_range = 1, unint8 fit_step = 1, double precision = 0);
	~LSTM();
};

class RNN : public Network {
	// параметрые, представл€ющие пам€ть предыдущей итерации сети
	double* h;

	// веса
	double* W_x;
	double* U_h;
	double* W_y;

	// смещени€
	double* b_h;
	double b_y;
};

// TODO class LSTM_lite

// TODO class GRU
