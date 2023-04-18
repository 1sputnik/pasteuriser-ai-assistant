#pragma once
#include "data_changer.h"

typedef unsigned __int16 unint16;

class RecurrentNeuron {
protected:
	// параметры сети
	unint16 epochs; // количество эпох сети
	unint16 input_range; // размер входной цепочки в сеть
	unint16 hidden_range; // размер скрытой цепочки в сети
	unint16 output_range; // размер выходной цепочки в сети
	double rate; // скорость оубчения сети
	double precision = 0.0000001; // точность предсказаний
	double e_predict = 0; // ошибка предсказания

	// параметрые, представляющие память сети
	double** h; // краткосрочная память


	// веса и смещения
	double* W_y; // веса для предскозаний
	double b_y; // смещение для предсказания 

public:
	virtual void fit(DataVector& train_data) = 0; // обучить сеть
	virtual void predict(DataVector& test_data, size_t add_predict_range) = 0; // выполнить предсказание

	void set_input_range(unint16 input_range);
	void set_hidden_range(unint16 hidden_range);
	void set_output_range(unint16 output_range);

	RecurrentNeuron(double rate = 0.001, unint16 epochs = 100, unint16 input_range = 1);
};

class LSTM : public RecurrentNeuron{
	// параметры сети
	string train_mode = "sequence";

	// параметрые, представляющие память сети
	double** C; // долгосрочная память 
	double* last_C_from_train = nullptr; // последний расчёт долгосрочной памяти при обучении 
	double* last_h_from_train = nullptr; // последний расчёт краткосрочной памяти при обучении 

	double* de_dF_futur; // распространение ошибки по вратам забывания следующей итерации сети
	double* de_dO_futur; // распространение ошибки по вратам выхода следующей итерации сети
	double* de_dG_futur; // распространение ошибки по вратам состояний следующей итерации сети
	double* de_dI_futur; // распространение ошибки по вратам входа следующей итерации
	double* de_dC_futur; // распространение ошибки по долгосрочной памяти следующей итерации
	double* forgate_futur; // врата забывания следующей итерации 

	// веса 
	double** W_f; // матрица весов врат забывания для входных значения
	double** W_i; // матрица весов врат входа для входных значения
	double** W_g; // матрица весов врат стостояний для входных значения
	double** W_o; // матрица весов врат выхода для входных значения

	double** U_f; // матрица весов врат забывания для краткосрочной памяти
	double** U_i; // матрица весов врат входа для краткосрочной памяти
	double** U_g; // матрица весов врат состояний для краткосрочной памяти
	double** U_o; // матрица весов врат выхода для краткосрочной памяти

	// промежуточные веса необходимы для расчёта метода обратного распространения 
	double** _W_f = nullptr; 
	double** _W_i = nullptr;
	double** _W_g = nullptr;
	double** _W_o = nullptr;

	double** _U_f = nullptr;
	double** _U_i = nullptr;
	double** _U_g = nullptr;
	double** _U_o = nullptr;

	double* _W_y = nullptr;

	// приватные функции
	void select_memory_for_temp_weight_and_biases();
	void copy_weight_and_biases();
	void free_temp_weigth_and_biases();

	void train(double* x, double y_real, size_t k = 0);
	double forecast(double* x, size_t k = 0);

public:
	virtual void fit(DataVector& train_data) override;
	virtual void predict(DataVector& test_data, size_t add_predict_range = 0) override;

	LSTM(double rate, unint16 epochs, unint16 input_range, unint16 hidden_range, string mode = "sequence");
	~LSTM();
};
