#pragma once

#include "NeuroSkeletons.h"
#include <fstream>
#include <string>
#include "WeightFiller.h"

class OCDFVanilaLSTM : public OCDFNeuron {
protected:
	//память сяти
	double_vector long_memory; // долгосрочная память сети
	double_vector short_memory; // краткосрочная память сети

	// врата
	double_vector forgate_gate; // врата забывания 
	double_vector input_gate; // врата входа
	double_vector output_gate; // врата выхода
	double_vector state_gate; // врата состояний 

	// веса и смещения
	double_vector W_f; // матрица весов врат забывания для входных значений
	double_vector W_i; // матрица весов врат входа для входных значения
	double_vector W_g; // матрица весов врат стостояний для входных значения
	double_vector W_o; // матрица весов врат выхода для входных значения

	double_vector U_f; // матрица весов врат забывания для краткосрочной памяти
	double_vector U_i; // матрица весов врат входа для краткосрочной памяти
	double_vector U_g; // матрица весов врат состояний для краткосрочной памяти
	double_vector U_o; // матрица весов врат выхода для краткосрочной памяти

	double_vector B_f; // вектора смещений врат забывания для краткосрочной памяти
	double_vector B_i; // вектора смещений врат входа для краткосрочной памяти
	double_vector B_g; // вектора смещений врат состояний для краткосрочной памяти
	double_vector B_o; // вектора смещений врат выхода для краткосрочной памяти

	double_vector W_y; // матрица весов выходного нейрона
	double_vector B_y; // вектор смещений выходного нейрона

	// временные веса и свещения 
	double_vector _W_f; // матрица весов врат забывания для входных значений
	double_vector _W_i; // матрица весов врат входа для входных значения
	double_vector _W_g; // матрица весов врат стостояний для входных значения
	double_vector _W_o; // матрица весов врат выхода для входных значения

	double_vector _U_f; // матрица весов врат забывания для краткосрочной памяти
	double_vector _U_i; // матрица весов врат входа для краткосрочной памяти
	double_vector _U_g; // матрица весов врат состояний для краткосрочной памяти
	double_vector _U_o; // матрица весов врат выхода для краткосрочной памяти

	double_vector _B_f; // вектора смещений врат забывания для краткосрочной памяти
	double_vector _B_i; // вектора смещений врат входа для краткосрочной памяти
	double_vector _B_g; // вектора смещений врат состояний для краткосрочной памяти
	double_vector _B_o; // вектора смещений врат выхода для краткосрочной памяти

	double_vector _W_y; // матрица весов выходного нейрона
	double_vector _B_y; // вектор смещений выходного нейрона

	// блок параметров для обучения
	double_vector de_dforgate; // распространение ошибки по вратам забывания из будущего
	double_vector de_dinput; // распространение ошибки по вратам входа из будущего
	double_vector de_dstate; // распространение ошибки по вратам состояний из будущего
	double_vector de_doutput; // распространение ошибки по вратам состояний следующей из будущего 
	double_vector de_dlongmem; // распространение ошибки по долгосрочной памяти из будущего
	double_vector forgate_futur; // врата забывания из будущего 
	double_vector past_long_memory; // предыдущее значение долгосрочной памяти
	double_vector past_short_memory; // предыдущее значение краткосрочной памяти


	// функции для работы с будующим
	virtual void clear_temp_vectors_for_learning();
	virtual void clear_futur_error();

	// функции для работы с весами
	virtual void create_weights();
	virtual void select_memory_for_temp_weight();
	virtual void copy_weight();
	virtual void free_temp_weigth();

	virtual void count_short_memory(vector<OCDF>& samples);
	virtual vector<double> forecast(vector<OCDF>& samples);
	virtual void learn(vector<OCDF>& samples, vector<OCDF>& etalons) override;

public:
	OCDFVanilaLSTM();
	OCDFVanilaLSTM(int input_range, int hidden_range, int output_range);
	OCDFVanilaLSTM(int input_range, int hidden_range, int output_range, AbstAct* activator);
	OCDFVanilaLSTM(int input_range, int hidden_range, int output_range, AbstAct* main_act, AbstAct* additional_act);

	virtual void fit(vector<OCDF>& data) override;
	virtual vector<OCDF> predict(vector<OCDF>& data) override;

	virtual void save_model(std::string file_name) override;
	virtual void load_model(std::string file_name) override;

	virtual std::string get_class_name() override;
};
