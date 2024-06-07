#include "VanilaLSTM.h"

typedef std::vector<std::vector<double>> double_matrix;

void OCDFVanilaLSTM::count_short_memory(vector<OCDF>& samples) {
	int& in = this->input_range;
	int& hid = this->hidden_range;

	for (int i = 0; i < hid; i++) {
		for (int j = 0; j < in; j++) {
			forgate_gate[i] += W_f[i * in + j] * samples[j].value;
			input_gate[i] += W_i[i * in + j] * samples[j].value;
			output_gate[i] += W_o[i * in + j] * samples[j].value;
			state_gate[i] += W_g[i * in + j] * samples[j].value;
		}
		for (int j = 0; j < hid; j++) {
			forgate_gate[i] += U_f[i * hid + j] * past_short_memory[j];
			input_gate[i] += U_i[i * hid + j] * past_short_memory[j];
			output_gate[i] += U_o[i * hid + j] * past_short_memory[j];
			state_gate[i] += U_g[i * hid + j] * past_short_memory[j];
		}
		forgate_gate[i] = main_activator->activate(forgate_gate[i] + B_f[i]); // sigm
		input_gate[i] = main_activator->activate(input_gate[i] + B_i[i]); // sigm
		output_gate[i] = main_activator->activate(output_gate[i] + B_o[i]); // sigm
		state_gate[i] = additional_activator->activate(state_gate[i] + B_g[i]); // tanh

		// расчитываем новую долгосрочную память
		long_memory[i] = forgate_gate[i] * past_long_memory[i] + input_gate[i] * state_gate[i];
		// расчитываем новую краткосрочную память
		short_memory[i] = output_gate[i] * additional_activator->activate(long_memory[i]); // tanh
	}
}

vector<double> OCDFVanilaLSTM::forecast(vector<OCDF>& samples) {
	this->count_short_memory(samples);

	// рассчитываем прогноз
	double_vector predicts = create_hollow_vector(this->output_range);
	for (size_t i = 0; i < this->output_range; i++) {
		for (size_t j = 0; j < this->hidden_range; j++)
			predicts[i] += W_y[j * output_range + i] * this->short_memory[j];
		predicts[i] = this->main_activator->activate(predicts[i] + B_y[i]);
	}

	return predicts;
}

void OCDFVanilaLSTM::learn(vector<OCDF>& samples, vector<OCDF>& etalons) {
	double e = 0;
	double_vector predicts = create_hollow_vector(this->output_range);
	for (size_t i = 0; i < this->output_range; i++) {
		for (size_t j = 0; j < this->hidden_range; j++)
			predicts[i] += W_y[j * output_range + i] * this->short_memory[j];
		predicts[i] = this->main_activator->activate(predicts[i] + B_y[i]);
		e += 0.5 * (etalons[i].value - predicts[i]) * (etalons[i].value - predicts[i]);
	}
	e_predict += e / double(output_range);

	double_vector de_dy(output_range);
	for (size_t i = 0; i < this->output_range; i++) {
		de_dy[i] = (predicts[i] - etalons[i].value) * this->main_activator->dif_activate_per_activate(predicts[i]);
	}
	predicts.clear();

	double_vector de_dh = create_hollow_vector(hidden_range);
	double_vector de_dC(hidden_range);
	for (int i = 0; i < this->hidden_range; i++) {
		for (size_t j = 0; j < this->output_range; j++) {
			de_dh[i] += de_dy[j] * _W_y[i * output_range + j];
		}
		for (int j = 0; j < this->hidden_range; j++) {
			de_dh[i] += de_dforgate[j] * _U_f[i * hidden_range + j]
				+ de_dstate[j] * _U_g[i * hidden_range + j]
				+ de_dinput[j] * _U_i[i * hidden_range + j]
				+ de_doutput[j] * _U_o[i * hidden_range + j];
		}
		de_doutput[i] = de_dh[i] 
			* additional_activator->activate(long_memory[i]) 
			* main_activator->dif_activate_per_activate(output_gate[i]);
		de_dC[i] = de_dh[i] * output_gate[i] 
			* additional_activator->dif_activate(long_memory[i])
			+ de_dlongmem[i] * forgate_futur[i];
		de_dforgate[i] = de_dC[i] 
			* past_long_memory[i] 
			* main_activator->dif_activate_per_activate(forgate_gate[i]);
		de_dinput[i] = de_dC[i] 
			* state_gate[i] 
			* main_activator->dif_activate_per_activate(input_gate[i]);
		de_dstate[i] = de_dC[i] 
			* input_gate[i] 
			* additional_activator->dif_activate_per_activate(state_gate[i]);
		for (int j = 0; j < this->input_range; j++) {
			W_i[i * input_range + j] -= learning_rate * de_dinput[i] * samples[j].value;
			W_f[i * input_range + j] -= learning_rate * de_dforgate[i] * samples[j].value;
			W_o[i * input_range + j] -= learning_rate * de_doutput[i] * samples[j].value;
			W_g[i * input_range + j] -= learning_rate * de_dstate[i] * samples[j].value;
		}
		for (int j = 0; j < this->hidden_range; j++) {
			U_i[i * hidden_range + j] -= learning_rate * de_dinput[i] * past_short_memory[j];
			U_f[i * hidden_range + j] -= learning_rate * de_dforgate[i] * past_short_memory[j];
			U_o[i * hidden_range + j] -= learning_rate * de_doutput[i] * past_short_memory[j];
			U_g[i * hidden_range + j] -= learning_rate * de_dstate[i] * past_short_memory[j];
		}
		B_i[i] -= learning_rate * de_dinput[i];
		B_o[i] -= learning_rate * de_doutput[i];
		B_f[i] -= learning_rate * de_dforgate[i];
		B_g[i] -= learning_rate * de_dstate[i];
		for (size_t j = 0; j < output_range; j++) {
			W_y[i * output_range + j] -= learning_rate * de_dy[j] * short_memory[i];
		}
	}
	for (size_t i = 0; i < output_range; i++) {
		B_y[i] -= learning_rate * de_dy[i];
	}
}

void OCDFVanilaLSTM::fit(vector<OCDF>& data) {
	std::cout << "prelearning per time = ";
	std::clock_t prestart, prefinish;
	prestart = clock();
	
	scaler.scale(data);
	long long work_size = data.size() - (this->input_range + this->output_range - 1);
	this->select_memory_for_temp_weight();
	
	prefinish = clock();
	std::cout << (double)(prefinish - prestart) / CLOCKS_PER_SEC << " sec\n";

	e_predict = this->target_error;
	for (size_t eps = 0; eps < this->epochs && e_predict >= this->target_error; eps++) {
		std::cout << "epoch = " << eps + 1 << "/" << this->epochs << " ";
		std::clock_t start, finish;
		start = clock();

		e_predict = 0;

		this->copy_weight();

		std::ofstream save_state_file("state.vanila.lstm", std::ios::binary);

		for (size_t i = 0; i < work_size; i++) {
			vector<OCDF> samples = sub_vector(data, input_range, i);
			past_long_memory = long_memory;
			past_short_memory = short_memory;
			this->count_short_memory(samples);
			save_state_file.seekp(0);
		}

		save_state_file.close();
		std::ifstream load_state_file ("state.vanila.lstm", std::ios::binary);

		for (int i = work_size - 1; i >= 0; i--) {
			vector<OCDF> samples = sub_vector(data, input_range, i);
			vector<OCDF> etalons = sub_vector(data, output_range, i + input_range);
			this->learn(samples, etalons);
		}

		load_state_file.close();

		finish = clock();
		std::cout << "| time = " << (double)(finish - start) / CLOCKS_PER_SEC << " sec "
			<< "| E = " << e_predict / 2 << "\n";
	}
	std::cout << "postlearning per time = ";
	std::clock_t poststart, postfinish;
	poststart = clock();

	this->clear_temp_vectors_for_learning();
	this->free_temp_weigth();
	scaler.unscale(data);

	postfinish = clock();
	std::cout << (double)(postfinish - poststart) / CLOCKS_PER_SEC << " sec\n";
}

vector<OCDF> OCDFVanilaLSTM::predict(vector<OCDF>& data) {
	this->scaler.scale(data);

	vector<OCDF> predicts (data.size() - this->input_range + this->output_range);

	int dist = data[1].time - data[0].time;

	if (past_short_memory.size() == 0) {
		past_short_memory = create_hollow_vector(this->hidden_range);
		past_long_memory = create_hollow_vector(this->hidden_range);
	}

	bool out_flag = true;
	for (size_t i = 0; i < data.size() && out_flag; i = i + this->output_range) {
		if (i > data.size() - this->input_range) {
			i = data.size() - this->input_range;
			out_flag = false;
		}
		vector<OCDF> samples = sub_vector(data, this->input_range, i);
		past_short_memory = short_memory;
		past_long_memory = long_memory;
		this->count_short_memory(samples);
		vector<double> resalts = this->forecast(samples);
		for (int j = 0; j < this->output_range; j++) {
			predicts[i + j].cid = data[0].cid;
			predicts[i + j].time = data[0].time + dist * (i + this->input_range + j);
			predicts[i + j].value = resalts[j];
		}
	}

	this->scaler.unscale(predicts);
	this->scaler.unscale(data);
	return predicts;
};

void OCDFVanilaLSTM::save_model(std::string file_name) {
	std::ofstream save_file(file_name, std::ios::binary);

	save_file.write((char*)&input_range, sizeof(input_range));
	save_file.write((char*)&hidden_range, sizeof(hidden_range));
	save_file.write((char*)&output_range, sizeof(output_range));
	save_file.write((char*)&epochs, sizeof(epochs));
	save_file.write((char*)&learning_rate, sizeof(learning_rate));
	save_file.write((char*)&target_error, sizeof(target_error));

	for(size_t i = 0; i < this->hidden_range; i++) {
		for (size_t j = 0; j < this->input_range; j++) {
			save_file.write((char*)&W_f[i * input_range + j], sizeof(W_f[i * input_range + j]));
			save_file.write((char*)&W_i[i * input_range + j], sizeof(W_i[i * input_range + j]));
			save_file.write((char*)&W_o[i * input_range + j], sizeof(W_o[i * input_range + j]));
			save_file.write((char*)&W_g[i * input_range + j], sizeof(W_g[i * input_range + j]));
		}
		for (size_t j = 0; j < this->hidden_range; j++) {
			save_file.write((char*)&U_f[i * hidden_range + j], sizeof(U_f[i * hidden_range + j]));
			save_file.write((char*)&U_i[i * hidden_range + j], sizeof(U_i[i * hidden_range + j]));
			save_file.write((char*)&U_o[i * hidden_range + j], sizeof(U_o[i * hidden_range + j]));
			save_file.write((char*)&U_g[i * hidden_range + j], sizeof(U_g[i * hidden_range + j]));
		}
		save_file.write((char*)&short_memory[i], sizeof(short_memory[i]));
	}

	for (size_t i = 0; i < this->output_range; i++) {
		for (size_t j = 0; j < this->hidden_range; j++) {
			save_file.write((char*)&W_y[j * output_range + i], sizeof(W_y[j * output_range + i]));
		}
		save_file.write((char*)&B_y[i], sizeof(B_y[i]));
	}

	std::pair<double, double> min_max_scale = this->scaler.get_scale();
	save_file.write((char*)&min_max_scale.second, sizeof(min_max_scale.second));
	save_file.write((char*)&min_max_scale.second, sizeof(min_max_scale.second));

	std::pair<double, double> min_max_config = this->scaler.get_configuration();
	save_file.write((char*)&min_max_config.second, sizeof(min_max_config.second));
	save_file.write((char*)&min_max_config.second, sizeof(min_max_config.second));

	save_file.close();
}

void OCDFVanilaLSTM::load_model(std::string file_name) {
	std::ifstream load_file(file_name, std::ios::binary);

	load_file.read((char*)&input_range, sizeof(input_range));
	load_file.read((char*)&hidden_range, sizeof(hidden_range));
	load_file.read((char*)&output_range, sizeof(output_range));
	load_file.read((char*)&epochs, sizeof(epochs));
	load_file.read((char*)&learning_rate, sizeof(learning_rate));
	load_file.read((char*)&target_error, sizeof(target_error));

	this->create_weights();

	for (size_t i = 0; i < this->hidden_range; i++) {
		for (size_t j = 0; j < this->input_range; j++) {
			load_file.read((char*)&W_f[i * input_range + j], sizeof(W_f[i * input_range + j]));
			load_file.read((char*)&W_i[i * input_range + j], sizeof(W_i[i * input_range + j]));
			load_file.read((char*)&W_o[i * input_range + j], sizeof(W_o[i * input_range + j]));
			load_file.read((char*)&W_g[i * input_range + j], sizeof(W_g[i * input_range + j]));
		}
		for (size_t j = 0; j < this->hidden_range; j++) {
			load_file.read((char*)&U_f[i * hidden_range + j], sizeof(U_f[i * hidden_range + j]));
			load_file.read((char*)&U_i[i * hidden_range + j], sizeof(U_i[i * hidden_range + j]));
			load_file.read((char*)&U_o[i * hidden_range + j], sizeof(U_o[i * hidden_range + j]));
			load_file.read((char*)&U_g[i * hidden_range + j], sizeof(U_g[i * hidden_range + j]));
		}
	}

	for (size_t i = 0; i < this->output_range; i++) {
		for (size_t j = 0; j < this->hidden_range; j++) {
			load_file.read((char*)&W_y[j * output_range + i], sizeof(W_y[j * output_range + i]));
		}
		load_file.read((char*)&B_y[i], sizeof(B_y[i]));
	}

	std::pair<double, double> min_max_scale;
	load_file.read((char*)&min_max_scale.second, sizeof(min_max_scale.second));
	load_file.read((char*)&min_max_scale.second, sizeof(min_max_scale.second));
	this->scaler.set_scale(min_max_scale);

	std::pair<double, double> min_max_config;
	load_file.read((char*)&min_max_config.second, sizeof(min_max_config.second));
	load_file.read((char*)&min_max_config.second, sizeof(min_max_config.second));
	this->scaler.configure(min_max_config);
	
	load_file.close();
}

void OCDFVanilaLSTM::clear_temp_vectors_for_learning() {
	this->de_dforgate.clear();
	this->de_dinput.clear();
	this->de_dlongmem.clear();
	this->de_dstate.clear();
	this->forgate_futur.clear();
	this->de_doutput.clear();
}

void OCDFVanilaLSTM::clear_futur_error() {
	de_dforgate = create_hollow_vector(hidden_range);
	de_dinput = create_hollow_vector(hidden_range);
	de_doutput = create_hollow_vector(hidden_range);
	de_dstate = create_hollow_vector(hidden_range);
	de_dlongmem = create_hollow_vector(hidden_range);
	forgate_futur = create_hollow_vector(hidden_range);
}

void OCDFVanilaLSTM::create_weights() {
	double deep = 100;
	
	forgate_gate = create_hollow_vector(hidden_range);
	input_gate = create_hollow_vector(hidden_range);
	output_gate = create_hollow_vector(hidden_range);
	state_gate = create_hollow_vector(hidden_range);

	W_i = create_random_vector(input_range * hidden_range, deep);
	W_f = create_random_vector(input_range * hidden_range, deep);
	W_o = create_random_vector(input_range * hidden_range, deep);
	W_g = create_random_vector(input_range * hidden_range, deep);

	U_i = create_random_vector(hidden_range * hidden_range, deep);
	U_f = create_random_vector(hidden_range * hidden_range, deep);
	U_o = create_random_vector(hidden_range * hidden_range, deep);
	U_g = create_random_vector(hidden_range * hidden_range, deep);

	B_i = create_hollow_vector(hidden_range);
	B_f = create_hollow_vector(hidden_range);
	B_o = create_hollow_vector(hidden_range);
	B_g = create_hollow_vector(hidden_range);

	W_y = create_random_vector(hidden_range * output_range, deep);
	B_y = create_hollow_vector(output_range);

	long_memory = create_hollow_vector(hidden_range);
	short_memory = create_hollow_vector(hidden_range);
}

void OCDFVanilaLSTM::select_memory_for_temp_weight() {
	_W_i = create_hollow_vector(input_range * hidden_range);
	_W_f = create_hollow_vector(input_range * hidden_range);
	_W_o = create_hollow_vector(input_range * hidden_range);
	_W_g = create_hollow_vector(input_range * hidden_range);
	
	_U_i = create_hollow_vector(hidden_range * hidden_range);
	_U_f = create_hollow_vector(hidden_range * hidden_range);
	_U_o = create_hollow_vector(hidden_range * hidden_range);
	_U_g = create_hollow_vector(hidden_range * hidden_range);

	_B_i = create_hollow_vector(hidden_range);
	_B_f = create_hollow_vector(hidden_range);
	_B_o = create_hollow_vector(hidden_range);
	_B_g = create_hollow_vector(hidden_range);

	_W_y = create_hollow_vector(hidden_range * output_range);
	_B_y = create_hollow_vector(output_range);

	this->de_dforgate = create_hollow_vector(hidden_range);
	this->de_dinput = create_hollow_vector(hidden_range);
	this->de_dlongmem = create_hollow_vector(hidden_range);
	this->de_dstate = create_hollow_vector(hidden_range);
	this->forgate_futur = create_hollow_vector(hidden_range);
	this->de_doutput = create_hollow_vector(hidden_range);
}

void OCDFVanilaLSTM::copy_weight() {
	_W_f = W_f;	_W_i = W_i;	_W_o = W_o;	_W_g = W_g;

	_U_f = U_f;	_U_i = U_i;	_U_o = U_o;	_U_g = U_g;

	_B_f = B_f;	_B_i = B_i;	_B_o = B_o;	_B_g = B_g;

	_B_y = B_y;	_W_y = W_y;
}

void OCDFVanilaLSTM::free_temp_weigth() {
	_W_i.clear();	_W_f.clear();	_W_o.clear();	_W_g.clear();

	_U_i.clear();	_U_f.clear();	_U_o.clear();	_U_g.clear();

	_B_i.clear();	_B_f.clear();	_B_o.clear();	_B_g.clear();

	_B_y.clear();	_W_y.clear();

	de_dforgate.clear();
	de_dinput.clear();
	de_dlongmem.clear();
	de_dstate.clear();
	forgate_futur.clear();
	de_doutput.clear();
}

OCDFVanilaLSTM::OCDFVanilaLSTM() : OCDFNeuron(1, 1 ,1) {}

OCDFVanilaLSTM::OCDFVanilaLSTM(int input_range, int hidden_range, int output_range) : OCDFNeuron(input_range, hidden_range, output_range) {
	this->create_weights();
}

OCDFVanilaLSTM::OCDFVanilaLSTM(int input_range, int hidden_range, int output_range, AbstAct* activator) : OCDFNeuron(input_range, hidden_range, output_range, activator) {
	this->create_weights();
}

OCDFVanilaLSTM::OCDFVanilaLSTM(int input_range, int hidden_range, int output_range, AbstAct* main_act, AbstAct* additional_act) : OCDFNeuron(input_range, hidden_range, output_range, main_act, additional_act) {
	this->create_weights();
};

std::string OCDFVanilaLSTM::get_class_name() {
	return "VanilaLSTM";
}
