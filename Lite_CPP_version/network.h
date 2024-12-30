#pragma once

#define ErrorInfo

#include "math_for_network.h"

typedef unsigned __int16 unint16;

class RecurrentNeuron {
protected:
	// параметры сети
	unint16 epochs; // количество эпох сети
	unint16 input_range; // размер входной цепочки в сеть
	unint16 hidden_range; // размер скрытой цепочки в сети
	unint16 output_range; // размер выходной цепочки в сети
	double learning_rate; 
	double predict_error = 0; // ошибка предсказания

	// параметрые, представляющие память сети
	double** h; // краткосрочная память

	// веса 
	double** W_y; // веса для предсказаний

public:
	virtual void fit(DataVector& train_data, double target_error = 0, long long batch_size = 0) = 0; // обучить сеть
	virtual void predict(DataVector& test_data) = 0; // выполнить предсказание

	RecurrentNeuron(double learning_rate, unint16 epochs, unint16 input_range);
};

class LSTM;

void load_model(LSTM& lstm, string file_name);
void dump_model(LSTM& lstm, string file_name);

class LSTM : public RecurrentNeuron{
	// параметрые, представляющие память сети
	double** C; // долгосрочная память 
	double* last_C_from_train = nullptr; // последний расчёт долгосрочной памяти при обучении 
	double* last_h_from_train = nullptr; // последний расчёт краткосрочной памяти при обучении 

	double* de_dF_future; // распространение ошибки по вратам забывания следующей итерации сети
	double* de_dO_future; // распространение ошибки по вратам выхода следующей итерации сети
	double* de_dG_future; // распространение ошибки по вратам состояний следующей итерации сети
	double* de_dI_future; // распространение ошибки по вратам входа следующей итерации
	double* de_dC_future; // распространение ошибки по долгосрочной памяти следующей итерации
	double* forgate_future; // врата забывания следующей итерации 

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

	double** _W_y = nullptr;

	// функции для работы с весами и будущими элементами
	void select_memory_for_temp_weight();
	void copy_weight();
	void free_temp_weigth();
	void clear_futur();

	// функции реализующие математический аппарат для работы сети
	void train(double* x, double* y_real, size_t k = 0);
	double* calculate_output(const size_t k);
	double calculate_error(double* y_predict, double* y_real);
	void calculate_gates(double*& forgate_gate, double*& x, double*& input_gate, double*& output_gate, double*& state_gate, const size_t k);

	double* forecast(double* x, size_t k = 0);

	// функции для сохранения и загрузки сети
	friend void load_model(LSTM& lstm, string file_name);
	friend void dump_model(LSTM& lstm, string file_name);

public:
	// функции, реализующие подачу данных в математический аппарат сети для нормального обучения или прогноза
	virtual void fit(DataVector& train_data, double target_error = 0, long long batch_size = 0) override;
	virtual void predict(DataVector& test_data) override;

	LSTM(double learning_rate, unint16 epochs, unint16 input_range, unint16 hidden_range, unint16 output_range);
	~LSTM();
};
