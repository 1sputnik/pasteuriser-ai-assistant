#include "TInVanilaLSTM.h"

typedef std::vector<std::vector<double>> double_matrix;

void TInOCDFVanilaLSTM::count_short_memory(vector<OCDF>& samples) {
	int& in = this->input_range;
	int& hid = this->hidden_range;

	for (int i = 0; i < hid; i++) {
		for (int j = 0; j < in; j++) {
			forgate_gate[i] += W_f[i * in + j] * samples[2 * j].value + V_f[i * in + j] * samples[2 * j + 1].value;
			input_gate[i] += W_i[i * in + j] * samples[2 * j].value + V_i[i * in + j] * samples[2 * j + 1].value;
			output_gate[i] += W_o[i * in + j] * samples[2 * j].value + V_o[i * in + j] * samples[2 * j + 1].value;
			state_gate[i] += W_g[i * in + j] * samples[2 * j].value + V_g[i * in + j] * samples[2 * j + 1].value;
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

vector<double> TInOCDFVanilaLSTM::forecast(vector<OCDF>& samples) {
	this->count_short_memory(samples);

	// рассчитываем прогноз
	double_vector predicts = create_hollow_vector(this->output_range);
	for (size_t i = 0; i < this->output_range; i++) {
		for (size_t j = 0; j < this->hidden_range; j++)
			predicts[i] += W_y[j * output_range + i] * this->short_memory[j];
		predicts[i] = this->main_activator->activate(predicts[i]);
	}

	return predicts;
}

void TInOCDFVanilaLSTM::learn(vector<OCDF>& samples, vector<OCDF>& etalons) {
	// рассчитываем прогноз
	double e = 0;
	double_vector predicts = create_hollow_vector(this->output_range);
	for (size_t i = 0; i < this->output_range; i++) {
		for (size_t j = 0; j < this->hidden_range; j++)
			predicts[i] += W_y[j * output_range + i] * this->short_memory[j];
		predicts[i] = this->main_activator->activate(predicts[i] + B_y[i]);
		e += 0.5 * (etalons[2 * i].value - predicts[i]) * (etalons[2 * i].value - predicts[i]);
	}
	e_predict += e / double(output_range);

	double_vector de_dy(output_range);
	for (size_t i = 0; i < this->output_range; i++) {
		de_dy[i] = (predicts[i] - etalons[2 * i].value) * this->main_activator->dif_activate_per_activate(predicts[i]);
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
			W_i[i * input_range + j] -= learning_rate * de_dinput[i] * samples[2 * j].value;
			W_f[i * input_range + j] -= learning_rate * de_dforgate[i] * samples[2 * j].value;
			W_o[i * input_range + j] -= learning_rate * de_doutput[i] * samples[2 * j].value;
			W_g[i * input_range + j] -= learning_rate * de_dstate[i] * samples[2 * j].value;

			V_i[i * input_range + j] -= learning_rate * de_dinput[i] * samples[2 * j + 1].value;
			V_f[i * input_range + j] -= learning_rate * de_dforgate[i] * samples[2 * j + 1].value;
			V_o[i * input_range + j] -= learning_rate * de_doutput[i] * samples[2 * j + 1].value;
			V_g[i * input_range + j] -= learning_rate * de_dstate[i] * samples[2 * j + 1].value;
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

void TInOCDFVanilaLSTM::fit(vector<OCDF>& data) {
	std::cout << "prelearning per time = ";
	std::clock_t prestart, prefinish;
	prestart = clock();
	
	scaler.scale(data);
	long long work_size = data.size() - ((2 * this->input_range) + (2 * this->output_range) - 1);
	this->select_memory_for_temp_weight();
	this->create_temp_vectors_for_learning();
	
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

		for (size_t i = 0; i < work_size; i = i + 2) {
			vector<OCDF> samples = sub_vector(data, 2 * input_range, i);
			past_long_memory = long_memory;
			past_short_memory = short_memory;
			this->count_short_memory(samples);
			this->save_state(save_state_file);
			save_state_file.seekp(0);
		}

		save_state_file.close();
		std::ifstream load_state_file ("state.vanila.lstm", std::ios::binary);

		OCDF temp = data[data.size() - 1];

		for (int i = work_size - 1; i >= 0; i = i - 2) {
			vector<OCDF> samples = sub_vector(data, 2 * input_range, i);
			vector<OCDF> etalons = sub_vector(data, 2 * output_range, i + 2 * input_range);
			this->load_state(load_state_file);
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

vector<OCDF> TInOCDFVanilaLSTM::predict(vector<OCDF>& samples) {
	scaler.scale(samples);

	vector<OCDF> predicts(((samples.size() - (2 * input_range)) / 2) + output_range);

	int dist = samples[2].time - samples[1].time;

	bool out_falg = true;
	for (size_t i = 0; i < samples.size() && out_falg; i = i + 2 * this->output_range) {
		if (i > samples.size() - 2 * this->input_range) {
			i = samples.size() - 2 * this->input_range;
			out_falg = false;
		}
		vector<OCDF> subsamples = sub_vector(samples, 2 * input_range, i);
		past_long_memory = long_memory;
		past_short_memory = short_memory;
		double_vector y_predict = forecast(subsamples);
		for (int j = 0; j < this->output_range; j++) {
			predicts[i / 2 + j].cid = samples[0].cid;
			predicts[i / 2 + j].time = samples[0].time + dist * (i + this->input_range + j);
			predicts[i / 2 + j].value = y_predict[j];
		}
	}

	scaler.unscale(samples);

	return predicts;
};

void TInOCDFVanilaLSTM::save_model(std::string file_name) {
	std::ofstream save_file(file_name, std::ios::binary);

	save_file.write((char*)&input_range, sizeof(input_range));
	save_file.write((char*)&hidden_range, sizeof(hidden_range));
	save_file.write((char*)&output_range, sizeof(output_range));
	save_file.write((char*)&epochs, sizeof(epochs));
	save_file.write((char*)&learning_rate, sizeof(learning_rate));
	save_file.write((char*)&target_error, sizeof(target_error));

	for (size_t i = 0; i < this->hidden_range; i++) {
		for (size_t j = 0; j < this->input_range; j++) {
			save_file.write((char*)&W_f[i * input_range + j], sizeof(W_f[i * input_range + j]));
			save_file.write((char*)&W_i[i * input_range + j], sizeof(W_i[i * input_range + j]));
			save_file.write((char*)&W_o[i * input_range + j], sizeof(W_o[i * input_range + j]));
			save_file.write((char*)&W_g[i * input_range + j], sizeof(W_g[i * input_range + j]));

			save_file.write((char*)&V_f[i * input_range + j], sizeof(V_f[i * input_range + j]));
			save_file.write((char*)&V_i[i * input_range + j], sizeof(V_i[i * input_range + j]));
			save_file.write((char*)&V_o[i * input_range + j], sizeof(V_o[i * input_range + j]));
			save_file.write((char*)&V_g[i * input_range + j], sizeof(V_g[i * input_range + j]));
		}
		for (size_t j = 0; j < this->hidden_range; j++) {
			save_file.write((char*)&U_f[i * hidden_range + j], sizeof(U_f[i * hidden_range + j]));
			save_file.write((char*)&U_i[i * hidden_range + j], sizeof(U_i[i * hidden_range + j]));
			save_file.write((char*)&U_o[i * hidden_range + j], sizeof(U_o[i * hidden_range + j]));
			save_file.write((char*)&U_g[i * hidden_range + j], sizeof(U_g[i * hidden_range + j]));
		}
		save_file.write((char*)&short_memory[i], sizeof(short_memory[i]));
		save_file.write((char*)&long_memory[i], sizeof(long_memory[i]));
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

void TInOCDFVanilaLSTM::load_model(std::string file_name) {
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

			load_file.read((char*)&V_f[i * input_range + j], sizeof(V_f[i * input_range + j]));
			load_file.read((char*)&V_i[i * input_range + j], sizeof(V_i[i * input_range + j]));
			load_file.read((char*)&V_o[i * input_range + j], sizeof(V_o[i * input_range + j]));
			load_file.read((char*)&V_g[i * input_range + j], sizeof(V_g[i * input_range + j]));
		}
		for (size_t j = 0; j < this->hidden_range; j++) {
			load_file.read((char*)&U_f[i * hidden_range + j], sizeof(U_f[i * hidden_range + j]));
			load_file.read((char*)&U_i[i * hidden_range + j], sizeof(U_i[i * hidden_range + j]));
			load_file.read((char*)&U_o[i * hidden_range + j], sizeof(U_o[i * hidden_range + j]));
			load_file.read((char*)&U_g[i * hidden_range + j], sizeof(U_g[i * hidden_range + j]));
		}
		load_file.read((char*)&short_memory[i], sizeof(short_memory[i]));
		load_file.read((char*)&long_memory[i], sizeof(long_memory[i]));
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

void TInOCDFVanilaLSTM::create_weights() {
	OCDFVanilaLSTM::create_weights();

	double deep = 100;

	V_i = create_random_vector(input_range * hidden_range, deep);
	V_f = create_random_vector(input_range * hidden_range, deep);
	V_o = create_random_vector(input_range * hidden_range, deep);
	V_g = create_random_vector(input_range * hidden_range, deep);
}

void TInOCDFVanilaLSTM::select_memory_for_temp_weight() {
	OCDFVanilaLSTM::copy_weight();

	_V_i = create_hollow_vector(input_range * hidden_range);
	_V_f = create_hollow_vector(input_range * hidden_range);
	_V_o = create_hollow_vector(input_range * hidden_range);
	_V_g = create_hollow_vector(input_range * hidden_range);
}

void TInOCDFVanilaLSTM::copy_weight() {
	OCDFVanilaLSTM::copy_weight();

	_V_f = V_f;	_V_i = V_i;	_V_o = V_o;	_V_g = V_g;
}

void TInOCDFVanilaLSTM::free_temp_weigth() {
	OCDFVanilaLSTM::copy_weight();

	_V_i.clear();	_V_f.clear();	_V_o.clear();	_V_g.clear();
}

TInOCDFVanilaLSTM::TInOCDFVanilaLSTM() : OCDFVanilaLSTM() { }

TInOCDFVanilaLSTM::TInOCDFVanilaLSTM(int input_range, int hidden_range, int output_range) : OCDFVanilaLSTM() {
	this->set_ranges(input_range, hidden_range, output_range);
	
	this->create_weights();
}

TInOCDFVanilaLSTM::TInOCDFVanilaLSTM(int input_range, int hidden_range, int output_range, AbstAct* activator) : OCDFVanilaLSTM() { 
	this->set_ranges(input_range, hidden_range, output_range);
	this->set_main_activator(activator);
	this->set_additional_activator(activator);
	
	this->create_weights();
}

TInOCDFVanilaLSTM::TInOCDFVanilaLSTM(int input_range, int hidden_range, int output_range, AbstAct* main_act, AbstAct* additional_act) : OCDFVanilaLSTM() { 
	this->set_ranges(input_range, hidden_range, output_range);
	this->set_main_activator(main_act);
	this->set_additional_activator(additional_act);
	
	this->create_weights();
}

std::string TInOCDFVanilaLSTM::get_class_name() {
	return "TInOCDFVanilaLSTM";
}
