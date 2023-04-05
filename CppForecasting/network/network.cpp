#include "network.h"

// ---------- ќбслуживание классов ----------

inline double sigmoid(double x) {
	return 1.0 / (1.0 + exp(-x));
}
inline double dif_sigmoid(double x) {
	return x * (1.0 - x);
}
inline double dif_tanh(double x) {
	x = tanh(x);
	return 1.0 - x * x;
}

inline void create_array(double*& arr, int size, double value = 0) {
	arr = new double [size];
	for (size_t i = 0; i < size; i++) {
		arr[i] = value;
	}
}
inline void create_2D_square_array(double**& arr, int size, double value = 0) {
	arr = new double* [size];
	for (size_t i = 0; i < size; i++) {
		arr[i] = new double[size];
		for (size_t j = 0; j < size; j++)
			arr[i][j] = value;
	}
}
inline void delete_2D_square_array(double**& arr, int size) {
	for (size_t i = 0; i < size; i++)
		delete[] arr[i];
	delete[] arr;
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

inline double* mul_matrix_by_vector(double** mat, double* vec, size_t size) {
	double* result_vec = new double[size] {0};
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) 
			result_vec[i] += mat[i][j] * vec[j];
	}
	return result_vec;
};
inline double mul_vec_by_vec(double* x, double* y, size_t size) {
	double result = 0;
	for (int i = 0; i < size; i++)
		result += x[i] * y[i];
	return result;
}


// ---------- Network ----------

Network::Network(double rate, unint8 epochs, unint16 input_range) {
	this->rate = rate;
	this->epochs = epochs;
	this->input_range = input_range;
}


// ---------- LSTM ----------

double LSTM::train(double* x, double y_real) {
	// ----- расчЄт модели -----

	// сохранение значений векторов пам€ти
	double* old_h = nullptr;
	copy_vector(old_h, h, this->input_range);
	double* old_C = nullptr;
	copy_vector(old_C, C, this->input_range);

	// создание врат (вентили) сети
	double* input_gate = new double[this->input_range];
	double* forgate_gate = new double[this->input_range];
	double* state_gate = new double[this->input_range];
	double* output_gate = new double[this->input_range];

	// буферы дл€ промежуточных рассчЄтов 
	double* W_mul_x;
	double* U_mul_h;

	// рассчЄт forgate_gate
	W_mul_x = mul_matrix_by_vector(W_f, x, this->input_range);
	U_mul_h = mul_matrix_by_vector(U_f, h, this->input_range);
	for (int i = 0; i < this->input_range; i++) {
		forgate_gate[i] = W_mul_x[i] + U_mul_h[i];
	}
	delete[] W_mul_x;
	delete[] U_mul_h;

	// рассчЄт input_gate
	W_mul_x = mul_matrix_by_vector(W_i, x, this->input_range);
	U_mul_h = mul_matrix_by_vector(U_i, h, this->input_range);
	for (int i = 0; i < this->input_range; i++) {
		input_gate[i] = W_mul_x[i] + U_mul_h[i];
	}
	delete[] W_mul_x;
	delete[] U_mul_h;

	// рассчЄт output_gate
	W_mul_x = mul_matrix_by_vector(W_o, x, this->input_range);
	U_mul_h = mul_matrix_by_vector(U_o, h, this->input_range);
	for (int i = 0; i < this->input_range; i++) {
		output_gate[i] = W_mul_x[i] + U_mul_h[i];
	}
	delete[] W_mul_x;
	delete[] U_mul_h;

	// рассчЄт state_gate
	W_mul_x = mul_matrix_by_vector(W_g, x, this->input_range);
	U_mul_h = mul_matrix_by_vector(U_g, h, this->input_range);
	for (int i = 0; i < this->input_range; i++) {
		state_gate[i] = W_mul_x[i] + U_mul_h[i];
	}
	delete[] W_mul_x;
	delete[] U_mul_h;

	// рассчЄт C - р€да состо€ний сети
	for (int i = 0; i < this->input_range; i++)
		C[i] = sigmoid(forgate_gate[i]) * C[i] + sigmoid(input_gate[i]) * state_gate[i];

	// рассчЄт h - пам€ть сети
	for (int i = 0; i < this->input_range; i++)
		h[i] = sigmoid(output_gate[i]) * tanh(C[i]);

	// рассчЄт предсказанного значени€
	double y_predict = mul_vec_by_vec(h, W_y, this->input_range);

	// ----- метод обратного распространени€ во времени -----
	double e = 0.5 * pow((y_real - sigmoid(y_predict)), 2);

	if (fabs(e) > this->precision) {
		double de_dy = (sigmoid(y_predict) - y_real) * dif_sigmoid(y_predict);

		double* de_dO = new double[this->input_range];
		double* de_dF = new double[this->input_range];
		double* de_dI = new double[this->input_range];
		double* de_dG = new double[this->input_range];
		double* de_dh = new double[this->input_range];
		double* de_dC = new double[this->input_range];
		for (int i = 0; i < this->input_range; i++) {
			de_dh[i] = de_dy * W_y[i];
			de_dO[i] = de_dh[i] * tanh(C[i]) * dif_sigmoid(output_gate[i]);
			de_dC[i] = de_dh[i] * sigmoid(output_gate[i]) * dif_tanh(C[i]);
			de_dF[i] = de_dC[i] * old_C[i] * dif_sigmoid(forgate_gate[i]);
			de_dI[i] = de_dC[i] * sigmoid(state_gate[i]) * dif_sigmoid(input_gate[i]);
			de_dG[i] = de_dC[i] * sigmoid(input_gate[i]) * dif_sigmoid(state_gate[i]);
			this->W_y[i] += this->rate * de_dy * h[i];
		}
		delete[] de_dh;
		delete[] de_dC;

		for (int i = 0; i < this->input_range; i++) {
			for (int j = 0; j < this->input_range; j++) {
				this->W_i[i][j] += this->rate * de_dI[i] * x[j];
				this->W_f[i][j] += this->rate * de_dF[i] * x[j];
				this->W_o[i][j] += this->rate * de_dO[i] * x[j];
				this->W_g[i][j] += this->rate * de_dG[i] * x[j];
				this->U_i[i][j] += this->rate * de_dI[i] * old_h[j];
				this->U_f[i][j] += this->rate * de_dF[i] * old_h[j];
				this->U_o[i][j] += this->rate * de_dO[i] * old_h[j];
				this->U_g[i][j] += this->rate * de_dG[i] * old_h[j];
			}
		}

		// завершение функции
		delete[] de_dO;
		delete[] de_dF;
		delete[] de_dI;
		delete[] de_dG;
	}
	delete[] output_gate;
	delete[] forgate_gate;
	delete[] input_gate;
	delete[] state_gate;
	delete[] old_C;
	delete[] old_h;

	return sigmoid(y_predict);
}
void LSTM::fit(DataVector& train_data) {
	DataVector temp_train_data;

	Scaler sc;
	sc.configure(100, 0);
	sc.scale(train_data);
	for (size_t i = 0; i < this->epochs; i++) {
		std::cout << "epoch = " << i + 1 << "/" << int(this->epochs) << " ";
		std::clock_t start, finish;
		start = clock();

		temp_train_data = train_data;
		for (size_t j = 0;; j = j + this->fit_step) {
			if (j + this->input_range >= train_data.size())
				break;
			double* x = create_input_vector(temp_train_data, this->input_range, j);
			double y_real = temp_train_data[j + this->input_range].value;

			double y_predict = train(x, y_real);

			temp_train_data[j + this->input_range].value = y_predict;
			delete[] x;
		}

		finish = clock();
		double duration = (double)(finish - start) / CLOCKS_PER_SEC;
		std::cout << "time = " << duration << "\n";
	}
	sc.unscale(temp_train_data);

	train_data = temp_train_data;
}
double LSTM::forecast(double* x, double y_predict) {
	// создание врат (вентили) сети
	double* input_gate = new double[this->input_range];
	double* forgate_gate = new double[this->input_range];
	double* state_gate = new double[this->input_range];
	double* output_gate = new double[this->input_range];

	// буферы дл€ промежуточных рассчЄтов 
	double* W_mul_x;
	double* U_mul_h;

	// рассчЄт forgate_gate
	W_mul_x = mul_matrix_by_vector(W_f, x, this->input_range);
	U_mul_h = mul_matrix_by_vector(U_f, h, this->input_range);
	for (int i = 0; i < this->input_range; i++) {
		forgate_gate[i] = W_mul_x[i] + U_mul_h[i];
	}
	delete[] W_mul_x;
	delete[] U_mul_h;

	// рассчЄт input_gate
	W_mul_x = mul_matrix_by_vector(W_i, x, this->input_range);
	U_mul_h = mul_matrix_by_vector(U_i, h, this->input_range);
	for (int i = 0; i < this->input_range; i++) {
		input_gate[i] = W_mul_x[i] + U_mul_h[i];
	}
	delete[] W_mul_x;
	delete[] U_mul_h;

	// рассчЄт output_gate
	W_mul_x = mul_matrix_by_vector(W_o, x, this->input_range);
	U_mul_h = mul_matrix_by_vector(U_o, h, this->input_range);
	for (int i = 0; i < this->input_range; i++) {
		output_gate[i] = W_mul_x[i] + U_mul_h[i];
	}
	delete[] W_mul_x;
	delete[] U_mul_h;

	// рассчЄт state_gate
	W_mul_x = mul_matrix_by_vector(W_g, x, this->input_range);
	U_mul_h = mul_matrix_by_vector(U_g, h, this->input_range);
	for (int i = 0; i < this->input_range; i++) {
		state_gate[i] = W_mul_x[i] + U_mul_h[i];
	}
	delete[] W_mul_x;
	delete[] U_mul_h;

	// рассчЄт C - р€да состо€ний сети
	for (int i = 0; i < this->input_range; i++)
		C[i] = sigmoid(forgate_gate[i]) * C[i] + sigmoid(input_gate[i]) * state_gate[i];

	// рассчЄт h - пам€ть сети
	for (int i = 0; i < this->input_range; i++)
		h[i] = sigmoid(output_gate[i]) * tanh(C[i]);

	delete[] output_gate;
	delete[] forgate_gate;
	delete[] input_gate;
	delete[] state_gate;

	// рассчЄт предсказанного значени€
	y_predict = mul_vec_by_vec(h, W_y, this->input_range);
	return sigmoid(y_predict);
}
void LSTM::predict(DataVector& test_data, size_t predict_range) {
	Scaler sc;
	sc.configure(100, 0);
	sc.scale(test_data);

	size_t dist = test_data.count_distance();

	test_data.resize(test_data.size() + predict_range);

	for (size_t i = 0; i < test_data.size(); i++) {
		if (i + this->input_range == test_data.size())
			break;
		double* x = create_input_vector(test_data, this->input_range, i);
		double y_predict = 0;
		y_predict = forecast(x, y_predict);
		delete[] x;
		test_data[i + this->input_range] = DATA(test_data[0].cid, test_data[0].time + dist * (i + this->input_range), y_predict);
	}

	sc.unscale(test_data);
}

LSTM::LSTM(double rate, unint8 epochs, unint16 input_range, unint8 fit_step, double precision) : Network(rate, epochs, input_range) {
	this->precision = precision;
	this->fit_step = fit_step;

	// параметрые, представл€ющие пам€ть предыдущей итерации сети
	h = new double[input_range] {0};
	C = new double[input_range] {0};

	// веса 
	create_2D_square_array(W_f, input_range, 0.01);
	create_2D_square_array(W_i, input_range, 0.01);
	create_2D_square_array(W_g, input_range, 0.01);
	create_2D_square_array(W_o, input_range, 0.01);

	create_2D_square_array(U_f, input_range, 0.01);
	create_2D_square_array(U_i, input_range, 0.01);
	create_2D_square_array(U_g, input_range, 0.01);
	create_2D_square_array(U_o, input_range, 0.01);

	create_array(W_y, this->input_range, 0.01);
}
LSTM::~LSTM() {
	// параметрые, представл€ющие пам€ть предыдущей итерации сети
	delete[] h;
	delete[] C;

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
}
