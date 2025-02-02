#pragma once

#include "NeuroSkeletons.h"
#include <fstream>
#include "VanilaLSTM.h"
#include "WeightFiller.h"

class TInOCDFVanilaLSTM : public OCDFVanilaLSTM {
	// веса 
	double_vector V_f; // матрица весов врат забывания для входных значений
	double_vector V_i; // матрица весов врат входа для входных значения
	double_vector V_g; // матрица весов врат стостояний для входных значения
	double_vector V_o; // матрица весов врат выхода для входных значения

	// временные веса
	double_vector _V_f; // матрица весов врат забывания для входных значений
	double_vector _V_i; // матрица весов врат входа для входных значения
	double_vector _V_g; // матрица весов врат стостояний для входных значения
	double_vector _V_o; // матрица весов врат выхода для входных значения

	// функции для работы с весами
	virtual void create_weights() override;
	virtual void select_memory_for_temp_weight() override;
	virtual void copy_weight() override;
	virtual void free_temp_weigth() override;

	// функции расчёта и обучения сети
	virtual void count_gates(vector<OCDF>& samples) override;
	virtual void learn(vector<OCDF>& samples, vector<OCDF>& etalons) override;

public:
	TInOCDFVanilaLSTM();
	TInOCDFVanilaLSTM(int input_range, int hidden_range, int output_range);
	TInOCDFVanilaLSTM(int input_range, int hidden_range, int output_range, AbstAct* activator);
	TInOCDFVanilaLSTM(int input_range, int hidden_range, int output_range, AbstAct* main_act, AbstAct* additional_act);

	virtual void fit(vector<OCDF>& data) override;
	virtual vector<OCDF> predict(vector<OCDF>& samples) override;

	virtual void save_model(std::string file_name) override;
	virtual void load_model(std::string file_name) override;

	virtual std::string get_class_name() override;
};
