#include "network.h"

// ---------- Обслуживание классов ----------

inline double sigm(double x) {
	return 1.0 / (1.0 + exp(-x));
}
inline double dif_sigm(double x) {
	return sigm(x) * (1.0 - sigm(x));
}
inline double dif_tanh(double x) {
	return 1.0 / pow(cosh(x), 2.0);
}

inline void create_array(double*& arr, int size, double value = 0) {
	arr = new double [size];
	for (size_t i = 0; i < size; i++) {
		arr[i] = value;
	}
}
inline void create_random_array(double*& arr, int size) {
	arr = new double[size];
	for (size_t i = 0; i < size; i++) {
		arr[i] = double((rand() % 2000) - 1000) / 1000.0;
	}
}
inline double* create_input_vector(DataVector& data, size_t cut_border, size_t b) {
	double* x = new double[cut_border];
	for (int i = 0; i < cut_border; i++)
		x[i] = data[i + b].value;
	return x;
};
inline void copy_vector(double*& in_data, double*& out_data, size_t size) {
	delete[] in_data;
	in_data = new double[size];
	for (int i = 0; i < size; i++)
		in_data[i] = out_data[i];
}

inline void create_2D_square_array(double**& arr, int size, double value = 0) {
	arr = new double* [size];
	for (size_t i = 0; i < size; i++) {
		arr[i] = new double[size];
		for (size_t j = 0; j < size; j++)
			arr[i][j] = value;
	}
}
inline void create_random_2D_square_array(double**& arr, int size) {
	arr = new double* [size];
	for (size_t i = 0; i < size; i++) {
		arr[i] = new double[size];
		for (size_t j = 0; j < size; j++)
			arr[i][j] = double((rand() % 2000) - 1000) / 1000.0;
	}
}
inline void delete_2D_square_array(double**& arr, int size) {
	for (size_t i = 0; i < size; i++)
		delete[] arr[i];
	delete[] arr;
}

// ---------- Network ----------

Neuron::Neuron(double rate, unint8 epochs, unint16 input_range) {
	this->rate = rate;
	this->epochs = epochs;
	this->input_range = input_range;
}


// ---------- LSTM ----------

double LSTM::train(double* x, double y_real, size_t k) {
	// ----- расчёт модели -----

	// создание врат (вентили) сети
	double* input_gate = new double[this->input_range] {0};
	double* forgate_gate = new double[this->input_range] {0};
	double* state_gate = new double[this->input_range] {0};
	double* output_gate = new double[this->input_range] {0};

	// расчёт врат и рогноза
	double y_predict = 0;
	for (int i = 0; i < this->input_range; i++) {
		for (int j = 0; j < this->input_range; j++) {
			forgate_gate[i] += W_f[i][j] * x[j] + U_f[i][j] * h[k][j];
			input_gate[i] += W_i[i][j] * x[j] + U_i[i][j] * h[k][j];
			output_gate[i] += W_o[i][j] * x[j] + U_o[i][j] * h[k][j];
			state_gate[i] += W_g[i][j] * x[j] + U_g[i][j] * h[k][j];
		}
		forgate_gate[i] += b_f[i];
		input_gate[i] += b_i[i];
		output_gate[i] += b_o[i];
		state_gate[i] += b_g[i];
		y_predict += W_y[i] * h[k + 1][i];
	} 

	double y = sigm(y_predict); // для отладки

	// ----- градиентный спуск -----
	double e = 0.5 * pow((y_real - sigm(y_predict)), 2);

	if (fabs(e) > this->precision) {
		double de_dy = (sigm(y_predict) - y_real) * dif_sigm(y_predict);

		double* de_dO = new double[this->input_range];
		double* de_dF = new double[this->input_range];
		double* de_dI = new double[this->input_range];
		double* de_dG = new double[this->input_range];
		double* de_dC = new double[this->input_range];
		double* de_dh = new double[this->input_range]{0};
		for (int i = 0; i < this->input_range; i++) {
			de_dh[i] += de_dy * W_y[i];
			for (int j = 0; j < this->input_range; j++) {
				de_dh[i] += de_dF_futur[j] * U_f[i][j] + de_dG_futur[j] * U_g[i][j]
					+ de_dI_futur[j] * U_i[i][j] + de_dO_futur[j] * U_o[i][j];
			}
			de_dO[i] = de_dh[i] * tanh(C[k + 1][i]) * dif_sigm(output_gate[i]);
			de_dC[i] = de_dh[i] * sigm(output_gate[i]) * dif_tanh(C[k + 1][i]) + de_dC_futur[i] * sigm(forgate_futur[i]);
			de_dF[i] = de_dC[i] * C[k][i] * dif_sigm(forgate_gate[i]);
			de_dI[i] = de_dC[i] * tanh(state_gate[i]) * dif_sigm(input_gate[i]);
			de_dG[i] = de_dC[i] * sigm(input_gate[i]) * dif_tanh(state_gate[i]);
			this->W_y[i] += this->rate * de_dy * h[k + 1][i];
			this->b_f[i] += this->rate * de_dF[i];
			this->b_i[i] += this->rate * de_dI[i];
			this->b_o[i] += this->rate * de_dO[i];
			this->b_g[i] += this->rate * de_dG[i];
		}
		copy_vector(de_dF_futur, de_dF, this->input_range);
		copy_vector(de_dO_futur, de_dO, this->input_range);
		copy_vector(de_dI_futur, de_dI, this->input_range);
		copy_vector(de_dG_futur, de_dG, this->input_range);
		copy_vector(de_dC_futur, de_dC, this->input_range);
		delete[] de_dh;
		delete[] de_dC;

		for (int i = 0; i < this->input_range; i++) {
			for (int j = 0; j < this->input_range; j++) {
				this->W_i[i][j] += this->rate * de_dI[i] * x[j];
				this->W_f[i][j] += this->rate * de_dF[i] * x[j];
				this->W_o[i][j] += this->rate * de_dO[i] * x[j];
				this->W_g[i][j] += this->rate * de_dG[i] * x[j];
				this->U_i[i][j] += this->rate * de_dI[i] * h[k][j];
				this->U_f[i][j] += this->rate * de_dF[i] * h[k][j];
				this->U_o[i][j] += this->rate * de_dO[i] * h[k][j];
				this->U_g[i][j] += this->rate * de_dG[i] * h[k][j];
			}
		}

		// завершение функции
		delete[] de_dO;
		delete[] de_dF;
		delete[] de_dI;
		delete[] de_dG;
	}
	else {
		for (int i = 0; i < this->input_range; i++) {
			de_dC_futur[i] = 0.0;
			de_dG_futur[i] = 0.0;
			de_dI_futur[i] = 0.0;
			de_dO_futur[i] = 0.0;
			de_dF_futur[i] = 0.0;
		}
	}
	copy_vector(forgate_futur, forgate_gate, this->input_range);
	delete[] output_gate;
	delete[] forgate_gate;
	delete[] input_gate;
	delete[] state_gate;

	return sigm(y_predict);
}
void LSTM::fit(DataVector& train_data) {
	Scaler sc;
	sc.configure(100, 0);
	sc.scale(train_data);

	// подготовка к обучению
	double** x = new double* [train_data.size()];
	double* y_real = new double [train_data.size()];
	C = new double* [train_data.size() + 1]; // + 1 для хранеия значений C[t + 1] (следующих значений)
	h = new double* [train_data.size() + 1];
	for (size_t i = 0; i <= train_data.size(); i++) {
		C[i] = new double[this->input_range] {0};
		h[i] = new double[this->input_range] {0};
		if (i < train_data.size())
			x[i] = new double[this->input_range];
	}
	int rand_b; // рандомное смещение

	// само обучение
	for (size_t i = 0; i < this->epochs; i++) {
		std::cout << "epoch = " << i + 1 << "/" << int(this->epochs) << " ";
		std::clock_t start, finish;
		start = clock();

		for (size_t j = 0; j < train_data.size() - 1; j++) {
			rand_b = rand() % (train_data.size() - this->input_range - 1);
			x[j] = create_input_vector(train_data, this->input_range, rand_b);
			y_real[j] = train_data[this->input_range + rand_b].value;
		}
		x[train_data.size() - 1] = create_input_vector(train_data, this->input_range, train_data.size() - this->input_range - 2);
		y_real[train_data.size() - 1] = train_data[train_data.size() - 1].value;

		for (size_t j = 0; j < train_data.size(); j++)
			double y_predict = forecast(x[j], j); // этот цикл нужен, чтобы расчитать каждые вектора C[j] и h[j] 

		for (int j = train_data.size() - 1; j >= 0; j--) 
			double y_predict = train(x[j], y_real[j], j); 

		for (int j = 0; j < train_data.size(); j++) 
			delete[] x[j];

		finish = clock();
		double duration = (double)(finish - start) / CLOCKS_PER_SEC;
		std::cout << "time = " << duration << " sec\n";
	}

	// завершение обучения
	copy_vector(h0_for_predict, h[train_data.size()], this->input_range);
	copy_vector(C0_for_predict, C[train_data.size()], this->input_range);
	for (int i = 0; i <= train_data.size() ; i++) {
		delete[] C[i];
		delete[] h[i];
	}
	delete[] C;
	delete[] h;
	delete[] x;
	delete[] y_real;

	sc.unscale(train_data);
}
double LSTM::forecast(double* x, size_t k) {
	// ----- расчёт модели -----
	double y_predict = 0;

	// создание врат (вентили) сети
	double* input_gate = new double[this->input_range] {0};
	double* forgate_gate = new double[this->input_range] {0};
	double* state_gate = new double[this->input_range] {0};
	double* output_gate = new double[this->input_range] {0};

	// расчёт врат и рогноза
	for (int i = 0; i < this->input_range; i++) {
		for (int j = 0; j < this->input_range; j++) {
			forgate_gate[i] += W_f[i][j] * x[j] + U_f[i][j] * h[k][j];
			input_gate[i] += W_i[i][j] * x[j] + U_i[i][j] * h[k][j];
			output_gate[i] += W_o[i][j] * x[j] + U_o[i][j] * h[k][j];
			state_gate[i] += W_g[i][j] * x[j] + U_g[i][j] * h[k][j];
		}
		forgate_gate[i] += b_f[i];
		input_gate[i] += b_i[i];
		output_gate[i] += b_o[i];
		state_gate[i] += b_g[i];
		C[k + 1][i] = sigm(forgate_gate[i]) * C[k][i] + sigm(input_gate[i]) * tanh(state_gate[i]);
		h[k + 1][i] = sigm(output_gate[i]) * tanh(C[k + 1][i]);
		y_predict += W_y[i] * h[k + 1][i];
	}

	delete[] output_gate;
	delete[] forgate_gate;
	delete[] input_gate;
	delete[] state_gate;

	return sigm(y_predict);
}
void LSTM::predict(DataVector& test_data, size_t predict_range) {
	Scaler sc;
	sc.configure(100, 0);
	sc.scale(test_data);

	// подготовка к предсказаниям
	size_t dist = test_data.count_distance();

	test_data.resize(test_data.size() + predict_range);

	C = new double* [2];
	h = new double* [2];
	for (int i = 0; i < 2; i++) {
		C[i] = new double[this->input_range];
		h[i] = new double[this->input_range];
	}
	copy_vector(C[0], C0_for_predict, this->input_range);
	copy_vector(h[0], h0_for_predict, this->input_range);
	delete[] C0_for_predict;
	delete[] h0_for_predict;

	// предсказания
	for (size_t i = 0; i < test_data.size() - this->input_range; i++) {
		double* x = create_input_vector(test_data, this->input_range, i);
		double y_predict = forecast(x);
		delete[] x;
		test_data[i + this->input_range] = DATA(test_data[0].cid, test_data[0].time + dist * (i + this->input_range), y_predict);
	}
	
	// завершение предсказаний
	for (int i = 0; i < 2; i++) {
		delete[] C[i];
		delete[] h[i];
	}
	delete[] C;
	delete[] h;

	sc.unscale(test_data);
}

LSTM::LSTM(double rate, unint8 epochs, unint16 input_range, double precision) : Neuron(rate, epochs, input_range) {
	this->precision = precision;

	// параметрые, представляющие память сети
	de_dF_futur = new double[input_range] {0};
	de_dO_futur = new double[input_range] {0};
	de_dG_futur = new double[input_range] {0};
	de_dI_futur = new double[input_range] {0};
	de_dC_futur = new double[input_range] {0};
	forgate_futur = new double[input_range] {0};

	// веса 
	create_random_2D_square_array(W_f, input_range);
	create_random_2D_square_array(W_i, input_range);
	create_random_2D_square_array(W_g, input_range);
	create_random_2D_square_array(W_o, input_range);

	create_random_2D_square_array(U_f, input_range);
	create_random_2D_square_array(U_i, input_range);
	create_random_2D_square_array(U_g, input_range);
	create_random_2D_square_array(U_o, input_range);

	create_random_array(W_y, input_range);

	// смещения
	create_random_array(b_i, input_range);
	create_random_array(b_g, input_range);
	create_random_array(b_f, input_range);
	create_random_array(b_o, input_range);
}
LSTM::~LSTM() {
	// параметрые, представляющие память сети
	delete[] de_dF_futur;
	delete[] de_dO_futur;
	delete[] de_dI_futur;
	delete[] de_dG_futur;
	delete[] de_dC_futur;
	delete[] forgate_futur;

	// веса 
	delete_2D_square_array(W_f, input_range);
	delete_2D_square_array(W_i, input_range);
	delete_2D_square_array(W_g, input_range);
	delete_2D_square_array(W_o, input_range);

	delete_2D_square_array(U_f, input_range);
	delete_2D_square_array(U_i, input_range);
	delete_2D_square_array(U_g, input_range);
	delete_2D_square_array(U_o, input_range);

	delete[] W_y;

	// смещения 
	delete[] b_f;
	delete[] b_o;
	delete[] b_g;
	delete[] b_i;
}
