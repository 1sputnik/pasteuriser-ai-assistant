#pragma once

#include <vector>
#include "ActFunctions.h"
#include "Scaler.h"
#include "../Common/DataFormats.h"

using std::vector;

typedef unsigned int unint;

class Neuron {
protected:
	int input_range; 
	int output_range;
	int hidden_range;
	int epochs;
	double target_error = 0; 
	double learning_rate = 1; 

	AbstractActivator* main_activator = nullptr;
	AbstractActivator* additional_activator = nullptr;

	void set_ranges(int input_range, int hidden_range, int output_range);

public:
	Scaler scaler;

	Neuron(int input_range, int hidden_range, int output_range);
	Neuron(int input_range, int hidden_range, int output_range, AbstAct* activator);
	Neuron(int input_range, int hidden_range, int output_range, AbstAct* main_act, AbstAct* additional_act);

	int get_input_range();
	int get_hidden_range();
	int get_output_range();
	void set_target_error(double target_error);
	double get_target_error();
	void set_learning_rate(double learning_rate);
	double get_learning_rate();
	void set_epochs(int epochs);
	int get_epochs();
	void set_main_activator(AbstractActivator* activator);
	void set_additional_activator(AbstractActivator* activator);

	virtual std::string get_class_name() = 0;
};

class OCDFNeuron : public Neuron {
protected:	
	void check_one_cid(vector<OCDF>& data);

	virtual void learn(vector<OCDF>& samples, vector<OCDF>& etalons) = 0;

public:
	bool configured_scaler = false;

	OCDFNeuron(int input_range, int hidden_range, int output_range) : Neuron(input_range, hidden_range, output_range) {}
	OCDFNeuron(int input_range, int hidden_range, int output_range, AbstAct* activator) : Neuron(input_range, hidden_range, output_range, activator) {}
	OCDFNeuron(int input_range, int hidden_range, int output_range, AbstAct* main_act, AbstAct* additional_act) : Neuron(input_range, hidden_range, output_range, main_act, additional_act) {}

	virtual void fit(vector<OCDF>& data) = 0;
	virtual vector<OCDF> predict(vector<OCDF>& samples) = 0;

	virtual void save_model(std::string file_name) = 0;
	virtual void load_model(std::string file_name) = 0;
};
