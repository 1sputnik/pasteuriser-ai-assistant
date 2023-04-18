#include "network.h"

// ---------- Обслуживание классов ----------

// математические функции
inline double sigm(double x) {
	return 1.0 / (1.0 + exp(-x));
}
inline double dif_sigm(double y) {
	return y * (1.0 - y);
}
inline double dif_tanh(double y) {
	return 1.0 - y * y;
}

// ветора (одномерные массивы)
inline void create_hollow_vector(double*& arr, int size) {
	arr = new double[size];
}
inline void randfill_vector(double*& arr, int size, double tiff) {
	// tiff - сколько нулей будет в значении весов: value = 0.<tiff><[0, rank -1]>
	double rank = 1000; // сколько знаков нам нужно при рандоме: [0, rank - 1]
	for (size_t i = 0; i < size; i++) 
		arr[i] = double((rand() % (2 * int(rank) + 1)) - int(1.0 * rank))
								/ (tiff * double(rank));
}
inline void create_random_vector(double*& arr, int size, double tiff = 1.0) {
	create_hollow_vector(arr, size);
	randfill_vector(arr, size, tiff);
}
inline void copy_vector(double*& in_data, double*& out_data, size_t size) {
	delete[] in_data;
	in_data = new double[size];
	for (int i = 0; i < size; i++)
		in_data[i] = out_data[i];
}

inline double* create_input_vector(DataVector& data, int cut_border, int b) {
	double* x = new double[cut_border];
	for (int i = 0; i < cut_border; i++)
		x[i] = data[i + b].value;
	return x;
};

// матрицы (двумерные массивы)
inline void create_hollow_matrix(double**& arr, size_t rows, size_t colls) {
	arr = new double* [rows];
	for (size_t i = 0; i < rows; i++)
		arr[i] = new double[colls];
}
inline void randfill_matrix(double**& arr, size_t rows, size_t colls, double tiff) {
	// tiff - сколько нулей будет в значении весов: value = 0.<tiff><[0, rank -1]>
	double rank = 1000; // сколько знаков нам нужно при рандоме: [0, rank - 1]
	for (size_t i = 0; i < rows; i++) 
		for (size_t j = 0; j < colls; j++)
			arr[i][j] = double((rand() % (2 * int(rank) + 1)) - int(1.0 * rank))
								/ (tiff * double(rank));
}
inline void fill_matrix(double**& arr, size_t rows, size_t colls, double value = 0) {
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < colls; j++)
			arr[i][j] = value;
}
inline void create_random_matrix(double**& arr, size_t rows, size_t colls, double tiff = 1.0) {
	create_hollow_matrix(arr, rows, colls);
	randfill_matrix(arr, rows, colls, tiff);
}
inline void create_matrix(double**& arr, size_t rows, size_t colls, double value = 0) {
	create_hollow_matrix(arr, rows, colls);
	fill_matrix(arr, rows, colls, value);
}
inline void copy_matrix(double**& in, double**& out, size_t rows, size_t colls) {
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < colls; j++)
			in[i][j] = out[i][j];
}
inline void delete_matrix(double**& arr, size_t rows) {
	for (size_t i = 0; i < rows; i++)
		delete[] arr[i];
	delete[] arr;
}


// ---------- Neuron ----------

RecurrentNeuron::RecurrentNeuron(double rate, unint16 epochs, unint16 input_range) {
	this->rate = rate;
	this->epochs = epochs;
	this->input_range = input_range;
	this->hidden_range = input_range;
	this->output_range = 1;
}

void RecurrentNeuron::set_hidden_range(unint16 hidden_range) {
	this->hidden_range = hidden_range;
}
void RecurrentNeuron::set_input_range(unint16 input_range) {
	this->input_range = input_range;
}
void RecurrentNeuron::set_output_range(unint16 output_range) {
	this->output_range = output_range;
}


// ---------- LSTM ----------

void LSTM::select_memory_for_temp_weight_and_biases() {
	// веса 
	create_hollow_matrix(_W_f, hidden_range, input_range);
	create_hollow_matrix(_W_i, hidden_range, input_range);
	create_hollow_matrix(_W_g, hidden_range, input_range);
	create_hollow_matrix(_W_o, hidden_range, input_range);

	create_hollow_matrix(_U_f, hidden_range, hidden_range);
	create_hollow_matrix(_U_i, hidden_range, hidden_range);
	create_hollow_matrix(_U_g, hidden_range, hidden_range);
	create_hollow_matrix(_U_o, hidden_range, hidden_range);

	create_hollow_vector(_W_y, hidden_range);
}
void LSTM::copy_weight_and_biases() {
	copy_matrix(_W_f, W_f, hidden_range, input_range);
	copy_matrix(_W_i, W_i, hidden_range, input_range);
	copy_matrix(_W_o, W_o, hidden_range, input_range);
	copy_matrix(_W_g, W_g, hidden_range, input_range);

	copy_matrix(_U_f, U_f, hidden_range, hidden_range);
	copy_matrix(_U_i, U_i, hidden_range, hidden_range);
	copy_matrix(_U_o, U_o, hidden_range, hidden_range);
	copy_matrix(_U_g, U_g, hidden_range, hidden_range);
	
	copy_vector(_W_y, W_y, hidden_range);
}
void LSTM::free_temp_weigth_and_biases() {
	// веса 
	delete_matrix(_W_f, hidden_range);
	delete_matrix(_W_i, hidden_range);
	delete_matrix(_W_g, hidden_range);
	delete_matrix(_W_o, hidden_range);

	delete_matrix(_U_f, hidden_range);
	delete_matrix(_U_i, hidden_range);
	delete_matrix(_U_g, hidden_range);
	delete_matrix(_U_o, hidden_range);

	delete[] _W_y;
}

void LSTM::train(double* x, double y_real, size_t k) {
	// ----- расчёт модели -----
	// создаём вентили
	double* forgate_gate = new double[this->hidden_range] {0};
	double* input_gate = new double[this->hidden_range] {0};
	double* output_gate = new double[this->hidden_range] {0};
	double* state_gate = new double[this->hidden_range] {0};

	// расчёт модели LSTM
	double y_predict = 0;
	for (int i = 0; i < this->hidden_range; i++) { // вычисляем значение элементов векторов вентилей 
		for (int j = 0; j < this->input_range; j++) {
			forgate_gate[i] += _W_f[i][j] * x[j];
			input_gate[i] += _W_i[i][j] * x[j];
			output_gate[i] += _W_o[i][j] * x[j];
			state_gate[i] += _W_g[i][j] * x[j];
		}
		for (int j = 0; j < this->hidden_range; j++) {
			forgate_gate[i] += _U_f[i][j] * h[k][j];
			input_gate[i] += _U_i[i][j] * h[k][j];
			output_gate[i] += _U_o[i][j] * h[k][j];
			state_gate[i] += _U_g[i][j] * h[k][j];
		}
		forgate_gate[i] = sigm(forgate_gate[i]); // прибавляем к векторам вентилей
		input_gate[i] = sigm(input_gate[i]); // вектора смещений b
		output_gate[i] = sigm(output_gate[i]);
		state_gate[i] = tanh(state_gate[i]);
		// выполяем предсказание
		y_predict += _W_y[i] * h[k + 1][i];
	}
	y_predict = sigm(y_predict); // приводим предсказание к необходимой форме

	// ----- градиентный спуск -----
	double e = 0.5 * (y_real - y_predict) * (y_real - y_predict);

	if (fabs(e) > this->precision) {
		e_predict += (y_real - y_predict) * (y_real - y_predict);
		double de_dy = (y_predict - y_real) * dif_sigm(y_predict);

		double* de_dO = new double[this->hidden_range];
		double* de_dF = new double[this->hidden_range];
		double* de_dI = new double[this->hidden_range];
		double* de_dG = new double[this->hidden_range];
		double* de_dC = new double[this->hidden_range];
		double* de_dh = new double[this->hidden_range];
		for (int i = 0; i < this->hidden_range; i++) {
			de_dh[i] = de_dy * _W_y[i];
			for (int j = 0; j < this->hidden_range; j++) {
				de_dh[i] += de_dF_futur[j] * _U_f[i][j] + de_dG_futur[j] * _U_g[i][j]
					+ de_dI_futur[j] * _U_i[i][j] + de_dO_futur[j] * _U_o[i][j];
			}
			de_dO[i] = de_dh[i] * tanh(C[k + 1][i]) * dif_sigm(output_gate[i]);
			de_dC[i] = de_dh[i] * output_gate[i] * dif_tanh(tanh(C[k + 1][i])) + de_dC_futur[i] * forgate_futur[i];
			de_dF[i] = de_dC[i] * C[k][i] * dif_sigm(forgate_gate[i]);
			de_dI[i] = de_dC[i] * state_gate[i] * dif_sigm(input_gate[i]);
			de_dG[i] = de_dC[i] * input_gate[i] * dif_tanh(state_gate[i]);
			this->W_y[i] -= this->rate * de_dy * h[k + 1][i];
			for (int j = 0; j < this->input_range; j++) {
				this->W_i[i][j] -= this->rate * de_dI[i] * x[j];
				this->W_f[i][j] -= this->rate * de_dF[i] * x[j];
				this->W_o[i][j] -= this->rate * de_dO[i] * x[j];
				this->W_g[i][j] -= this->rate * de_dG[i] * x[j];
			}
			for (int j = 0; j < this->hidden_range; j++) {
				this->U_i[i][j] -= this->rate * de_dI[i] * h[k][j];
				this->U_f[i][j] -= this->rate * de_dF[i] * h[k][j];
				this->U_o[i][j] -= this->rate * de_dO[i] * h[k][j];
				this->U_g[i][j] -= this->rate * de_dG[i] * h[k][j];
			}
		}

		// завершение функции
		copy_vector(de_dF_futur, de_dF, this->hidden_range);
		copy_vector(de_dO_futur, de_dO, this->hidden_range);
		copy_vector(de_dI_futur, de_dI, this->hidden_range);
		copy_vector(de_dG_futur, de_dG, this->hidden_range);
		copy_vector(de_dC_futur, de_dC, this->hidden_range);
		delete[] de_dh;
		delete[] de_dC;
		delete[] de_dO;
		delete[] de_dF;
		delete[] de_dI;
		delete[] de_dG;
	}
	else {
		for (int i = 0; i < this->hidden_range; i++) {
			de_dC_futur[i] = 0.0;
			de_dG_futur[i] = 0.0;
			de_dI_futur[i] = 0.0;
			de_dO_futur[i] = 0.0;
			de_dF_futur[i] = 0.0;
		}
	}
	copy_vector(forgate_futur, forgate_gate, this->hidden_range);
	
	// очищаем память, выделенную на вентили сети
	delete[] forgate_gate;
	delete[] input_gate;
	delete[] state_gate;
	delete[] output_gate;
}
void LSTM::fit(DataVector& train_data) {
	// создаём и конфигурируем скейлер
	Scaler sc;
	sc.configure(120, 0);

	// скейлим входные данные
	sc.scale(train_data);

	// смещение сети в зависимости от режима подачи входных цепочек
	size_t mode_bias;
	if (this->train_mode == "random")
		mode_bias = 0;
	else if (this->train_mode == "sequence")
		mode_bias = this->input_range;
	else {
		std::cerr << "\n!!! train mode is not define !!!\n";
		exit(0);
	}

	// подготовка к обучению
	double** x = new double* [train_data.size() - mode_bias]; // матрица входных цепочек для каждой итерации
	double* y_real = new double [train_data.size() - mode_bias]; // вектор реальных значений каждой итерации
	C = new double* [train_data.size() + 1 - mode_bias]; // + 1 для хранеия значений C[t + 1] (следующих значений)
	h = new double* [train_data.size() + 1 - mode_bias];
	for (size_t i = 0; i <= train_data.size() - mode_bias; i++) { // орагнизуем начальную долгосрочную и краткосрочную память
		C[i] = new double[this->hidden_range] {0};
		h[i] = new double[this->hidden_range] {0};
	}

	// выделяем память под промежуточные веса
	select_memory_for_temp_weight_and_biases();

	// обучение сети
	for (size_t i = 0; i < this->epochs; i++) {
		std::cout << "epoch = " << i + 1 << "/" << this->epochs << " ";
		std::clock_t start, finish;
		start = clock();

		// сохряняем текущие значения весов
		copy_weight_and_biases();

		// обнуляем среднюю ошибку эпохи предсказаний
		e_predict = 0;

		// формируем входные цепочки и их соотвесттвующие реальные значения, с которыми будет проводиться сравнение предсказаний
		if (this->train_mode == "random") {
			int rand_bias; // смещение для формирования входных цепочек
			for (int j = 0; j < train_data.size(); j++) {
				rand_bias = rand() % (train_data.size() - this->input_range - 1);
				x[j] = create_input_vector(train_data, this->input_range, rand_bias);
				y_real[j] = train_data[this->input_range + rand_bias].value;
			}
		}
		else if (this->train_mode == "sequence") {
			for (int j = 0; j < train_data.size() - mode_bias; j++) {
				x[j] = create_input_vector(train_data, mode_bias, j);
				y_real[j] = train_data[mode_bias + j].value;
			}
		}
		
		for (size_t j = 0; j < train_data.size() - mode_bias; j++) {
			forecast(x[j], j); // этот цикл нужен, чтобы расчитать каждые вектора C и h для каждой итерации 
		}
		
		for (int j = train_data.size() - 1 - mode_bias; j >= 0; j--) {
			train(x[j], y_real[j], j); // метод обратного распространения во времени		
			delete[] x[j]; // удаление для дальнейшего обновления
		}

		finish = clock();
		std::cout << "| time = " << (double)(finish - start) / CLOCKS_PER_SEC << " sec "
			<< "| E = " << e_predict / 2 << "\n";
	}

	// сохраняем память сети после последнего её расчёта при обучении
	copy_vector(last_C_from_train, C[train_data.size() - mode_bias], this->hidden_range);
	copy_vector(last_h_from_train, h[train_data.size() - mode_bias], this->hidden_range);

	// очищаем память
	free_temp_weigth_and_biases();
	for (int i = 0; i <= train_data.size() - mode_bias; i++) {
		delete[] C[i];
		delete[] h[i];
	}
	delete[] C;
	delete[] h;
	delete[] x;
	delete[] y_real;

	// восстанавливаем данные
	sc.unscale(train_data);
}

double LSTM::forecast(double* x, size_t k) {
	// создаём вентили
	double* forgate_gate = new double[this->hidden_range] {0};
	double* input_gate = new double[this->hidden_range] {0};
	double* output_gate = new double[this->hidden_range] {0};
	double* state_gate = new double[this->hidden_range] {0};

	// расчёт модели LSTM
	double y_predict = 0;
	for (int i = 0; i < this->hidden_range; i++) { // вычисляем значение элементов векторов вентилей 
		for (int j = 0; j < this->input_range; j++) { 
			forgate_gate[i] += W_f[i][j] * x[j];
			input_gate[i] += W_i[i][j] * x[j];   
			output_gate[i] += W_o[i][j] * x[j];  
			state_gate[i] += W_g[i][j] * x[j]; 
		}
		for (int j = 0; j < this->hidden_range; j++) { 
			forgate_gate[i] += U_f[i][j] * h[k][j]; 
			input_gate[i] += U_i[i][j] * h[k][j];
			output_gate[i] += U_o[i][j] * h[k][j]; 
			state_gate[i] += U_g[i][j] * h[k][j];   
		}
		forgate_gate[i] = sigm(forgate_gate[i]); // прибавляем к векторам вентилей
		input_gate[i] = sigm(input_gate[i]); 
		output_gate[i] = sigm(output_gate[i]);
		state_gate[i] = tanh(state_gate[i]);
		// расчитываем новую долгосрочную память
		C[k + 1][i] = forgate_gate[i] * C[k][i] + input_gate[i] * state_gate[i];
		// расчитываем новую краткосрочную память
		h[k + 1][i] = output_gate[i] * tanh(C[k + 1][i]);
		// выполяем предсказание
		y_predict += W_y[i] * h[k + 1][i];
	}
	y_predict = sigm(y_predict); // приводим предсказание к необходимой форме

	// очищаем память, выделенную на вентили сети
	delete[] forgate_gate;
	delete[] input_gate;
	delete[] state_gate;
	delete[] output_gate;

	// возвращаем предстказание сети
	return y_predict;
}
void LSTM::predict(DataVector& test_data, size_t predict_range) {
	// создаём и конфигурируем скейлер
	Scaler sc;
	sc.configure(120, 0);

	// скейлим входные данные
	sc.scale(test_data);

	// предполагается, что расстояние между точками по оси Х равное, поэтому вычисляем его
	size_t dist = test_data.count_distance();
	// расширяем данные, чтобы можно было поместить туда новые предсказанные значения
	test_data.resize(test_data.size() + predict_range);

	// создаём долгосрочную и краткосрочную память
	C = new double* [2];
	h = new double* [2];
	if (this->train_mode == "sequence") {
		C[0] = nullptr;
		h[0] = nullptr;
		copy_vector(C[0], last_C_from_train, this->hidden_range);
		copy_vector(h[0], last_h_from_train, this->hidden_range);
	}
	else if (this->train_mode == "random") {
		C[0] = new double [this->hidden_range] {0};
		h[0] = new double [this->hidden_range] {0};
	}
	delete[] last_C_from_train;
	delete[] last_h_from_train;
	C[1] = new double[this->hidden_range] {0};
	h[1] = new double[this->hidden_range] {0};

	// выполняем предсказания
	double* x = nullptr;
	for (size_t i = 0; i < test_data.size() - this->input_range; i++) {
		x = create_input_vector(test_data, this->input_range, i);
		double y_predict = forecast(x);
		double x_ = x[0];
		delete[] x;
		test_data[i + this->input_range].cid = test_data[0].cid;
		test_data[i + this->input_range].time = test_data[0].time + dist * (i + this->input_range);
		test_data[i + this->input_range].value = y_predict;
		copy_vector(C[0], C[1], this->hidden_range);
		copy_vector(h[0], h[1], this->hidden_range);
	}
	
	// очищаем долгосрочную и краткосрочную память
	delete[] C[1];
	delete[] C[0];
	delete[] C;

	delete[] h[1];
	delete[] h[0];
	delete[] h;

	// востанавливаем значиня из промежутка [0, 1] в нормальный промежуток
	sc.unscale(test_data);
}

LSTM::LSTM(double rate, unint16 epochs, unint16 input_range, unint16 hidden_range, string mode) : RecurrentNeuron(rate, epochs, input_range) {
	this->train_mode = mode;
	this->hidden_range = hidden_range;
	
	// параметрые, представляющие память сети
	de_dF_futur = new double[this->hidden_range] {0};
	de_dO_futur = new double[this->hidden_range] {0};
	de_dG_futur = new double[this->hidden_range] {0};
	de_dI_futur = new double[this->hidden_range] {0};
	de_dC_futur = new double[this->hidden_range] {0};
	forgate_futur = new double[this->hidden_range] {0};

	// веса 
	create_random_matrix(W_f, hidden_range, input_range);
	create_random_matrix(W_i, hidden_range, input_range);
	create_random_matrix(W_g, hidden_range, input_range);
	create_random_matrix(W_o, hidden_range, input_range);

	create_random_matrix(U_f, hidden_range, hidden_range);
	create_random_matrix(U_i, hidden_range, hidden_range);
	create_random_matrix(U_g, hidden_range, hidden_range);
	create_random_matrix(U_o, hidden_range, hidden_range);

	create_random_vector(W_y, hidden_range);
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
	delete_matrix(W_f, hidden_range);
	delete_matrix(W_i, hidden_range);
	delete_matrix(W_g, hidden_range);
	delete_matrix(W_o, hidden_range);

	delete_matrix(U_f, hidden_range);
	delete_matrix(U_i, hidden_range);
	delete_matrix(U_g, hidden_range);
	delete_matrix(U_o, hidden_range);

	delete[] W_y;
}
