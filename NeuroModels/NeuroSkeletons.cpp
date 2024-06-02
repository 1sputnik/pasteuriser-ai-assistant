#include "NeuroSkeletons.h"


Neuron::Neuron(int input_range, int hidden_range, int output_range) {
	this->set_ranges(input_range, hidden_range, output_range);
}

Neuron::Neuron(int input_range, int hidden_range, int output_range, AbstAct* activator) {
	this->set_ranges(input_range, hidden_range, output_range);
	this->set_main_activator(activator);
	this->set_additional_activator(activator);
}

Neuron::Neuron(int input_range, int hidden_range, int output_range, AbstAct* main_act, AbstAct* additional_act) {
	this->set_ranges(input_range, hidden_range, output_range);
	this->set_main_activator(main_act);
	this->set_additional_activator(additional_act);
}

void Neuron::set_ranges(int input_range, int hidden_range, int output_range) {
	this->input_range = input_range;
	this->hidden_range = hidden_range;
	this->output_range = output_range;
}

int Neuron::get_input_range() {
	return this->input_range;
}

int Neuron::get_hidden_range() {
	return this->hidden_range;
}

int Neuron::get_output_range() {
	return this->output_range;
}

void Neuron::set_target_error(double target_error) {
	this->target_error = target_error;
}

double Neuron::get_target_error() {
	return this->target_error;
}

void Neuron::set_learning_rate(double learning_rate) {
	this->learning_rate = learning_rate;
}

double Neuron::get_learning_rate() {
	return this->learning_rate;
}

void Neuron::set_main_activator(AbstractActivator* activator) {
	delete this->main_activator;
	this->main_activator = activator;
}

void Neuron::set_additional_activator(AbstractActivator* activator) {
	delete this->additional_activator;
	this->additional_activator = activator;
}

void Neuron::set_epochs(int epochs) {
	this->epochs = epochs;
}

int Neuron::get_epochs() {
	return this->epochs;
}

void OCDFNeuron::check_one_cid(vector<OCDF>& data) {
	for (size_t i = 1; i < data.size(); i++) {
		if (data[0].cid != data[i].cid) {
			throw std::invalid_argument("data contains differen cids");
		}
	}
}
