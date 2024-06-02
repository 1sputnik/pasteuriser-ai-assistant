#include "network.h"

// ---------- Neuron ----------

RecurrentNeuron::RecurrentNeuron(double learning_rate, unint16 epochs, unint16 input_range) {
	this->learning_rate = learning_rate;
	this->epochs = epochs;
	this->input_range = input_range;
}

// ---------- LSTM ----------

void LSTM::select_memory_for_temp_weight() {
	create_empty_matrix(_W_f, hidden_range, input_range);
	create_empty_matrix(_W_i, hidden_range, input_range);
	create_empty_matrix(_W_g, hidden_range, input_range);
	create_empty_matrix(_W_o, hidden_range, input_range);

	create_empty_matrix(_U_f, hidden_range, hidden_range);
	create_empty_matrix(_U_i, hidden_range, hidden_range);
	create_empty_matrix(_U_g, hidden_range, hidden_range);
	create_empty_matrix(_U_o, hidden_range, hidden_range);

	create_empty_matrix(_W_y, output_range, hidden_range);
}

void LSTM::copy_weight() {
	copy_matrix(_W_f, W_f, hidden_range, input_range);
	copy_matrix(_W_i, W_i, hidden_range, input_range);
	copy_matrix(_W_o, W_o, hidden_range, input_range);
	copy_matrix(_W_g, W_g, hidden_range, input_range);

	copy_matrix(_U_f, U_f, hidden_range, hidden_range);
	copy_matrix(_U_i, U_i, hidden_range, hidden_range);
	copy_matrix(_U_o, U_o, hidden_range, hidden_range);
	copy_matrix(_U_g, U_g, hidden_range, hidden_range);
	
	copy_matrix(_W_y, W_y, output_range, hidden_range);
}

void LSTM::free_temp_weigth() { 
	delete_matrix(_W_f, hidden_range);
	delete_matrix(_W_i, hidden_range);
	delete_matrix(_W_g, hidden_range);
	delete_matrix(_W_o, hidden_range);

	delete_matrix(_U_f, hidden_range);
	delete_matrix(_U_i, hidden_range);
	delete_matrix(_U_g, hidden_range);
	delete_matrix(_U_o, hidden_range);

	delete_matrix(_W_y, output_range);
}

void LSTM::clear_futur() {
	for (size_t i = 0; i < this->hidden_range; i++) {
		this->forgate_future[i] = 0;
		this->de_dC_future[i] = 0;
		this->de_dF_future[i] = 0;
		this->de_dG_future[i] = 0;
		this->de_dI_future[i] = 0;
		this->de_dO_future[i] = 0;
	}
}

void LSTM::train(double* x, double* y_real, size_t k) {
	// ----- расчёт модели -----
	// создаём вентили и рассчитываем их
	double* forgate_gate = new double[this->hidden_range] {0};
	double* input_gate = new double[this->hidden_range] {0};
	double* output_gate = new double[this->hidden_range] {0};
	double* state_gate = new double[this->hidden_range] {0};
	calculate_gates(forgate_gate, x, input_gate, output_gate, state_gate, k);

	// выполяем предсказания
	double* y_predict = calculate_output(k);
	double error = calculate_error(y_predict, y_real);
	predict_error += error / double(output_range);

	// ----- градиентный спуск -----
	double error_presition = 0.00000001;
	if (fabs(error / double(output_range)) > error_presition) {
		double* de_dy = new double[this->output_range];
		for (size_t i = 0; i < this->output_range; i++)
			de_dy[i] = (y_predict[i] - y_real[i]) * dif_sigm(y_predict[i]);
		delete[] y_predict;

		double* de_dO = new double[this->hidden_range];
		double* de_dF = new double[this->hidden_range];
		double* de_dI = new double[this->hidden_range];
		double* de_dG = new double[this->hidden_range];
		double* de_dC = new double[this->hidden_range];
		double* de_dh = new double[this->hidden_range] {0};
		for (int i = 0; i < this->hidden_range; i++) {
			for (size_t j = 0; j < this->output_range; j++)
				de_dh[i] += de_dy[j] * _W_y[j][i];
			for (int j = 0; j < this->hidden_range; j++) {
				de_dh[i] += de_dF_future[j] * _U_f[i][j] + de_dG_future[j] * _U_g[i][j]
					+ de_dI_future[j] * _U_i[i][j] + de_dO_future[j] * _U_o[i][j];
			}
			de_dO[i] = de_dh[i] * tanh(C[k + 1][i]) * dif_sigm(output_gate[i]);
			de_dC[i] = de_dh[i] * output_gate[i] * dif_tanh(tanh(C[k + 1][i])) + de_dC_future[i] * forgate_future[i];
			de_dF[i] = de_dC[i] * C[k][i] * dif_sigm(forgate_gate[i]);
			de_dI[i] = de_dC[i] * state_gate[i] * dif_sigm(input_gate[i]);
			de_dG[i] = de_dC[i] * input_gate[i] * dif_tanh(state_gate[i]);
			for (int j = 0; j < this->input_range; j++) {
				this->W_i[i][j] -= this->learning_rate * de_dI[i] * x[j];
				this->W_f[i][j] -= this->learning_rate * de_dF[i] * x[j];
				this->W_o[i][j] -= this->learning_rate * de_dO[i] * x[j];
				this->W_g[i][j] -= this->learning_rate * de_dG[i] * x[j];
			}
			for (int j = 0; j < this->hidden_range; j++) {
				this->U_i[i][j] -= this->learning_rate * de_dI[i] * h[k][j];
				this->U_f[i][j] -= this->learning_rate * de_dF[i] * h[k][j];
				this->U_o[i][j] -= this->learning_rate * de_dO[i] * h[k][j];
				this->U_g[i][j] -= this->learning_rate * de_dG[i] * h[k][j];
			}
			for (size_t j = 0; j < this->output_range; j++)
				this->W_y[j][i] -= this->learning_rate * de_dy[j] * h[k + 1][i];
		}

		// завершение функции
		copy_vector(de_dF_future, de_dF, this->hidden_range);
		copy_vector(de_dO_future, de_dO, this->hidden_range);
		copy_vector(de_dI_future, de_dI, this->hidden_range);
		copy_vector(de_dG_future, de_dG, this->hidden_range);
		copy_vector(de_dC_future, de_dC, this->hidden_range);
		delete[] de_dy;
		delete[] de_dh;
		delete[] de_dC;
		delete[] de_dO;
		delete[] de_dF;
		delete[] de_dI;
		delete[] de_dG;
	}
	else {
		for (int i = 0; i < this->hidden_range; i++) {
			de_dC_future[i] = 0.0;
			de_dG_future[i] = 0.0;
			de_dI_future[i] = 0.0;
			de_dO_future[i] = 0.0;
			de_dF_future[i] = 0.0;
		}
	}
	copy_vector(forgate_future, forgate_gate, this->hidden_range);

	// очищаем память, выделенную на вентили сети
	delete[] forgate_gate;
	delete[] input_gate;
	delete[] state_gate;
	delete[] output_gate;
}

double* LSTM::calculate_output(const size_t k)
{
	double* y_predict = new double [this->output_range] {0};
	for (size_t i = 0; i < this->output_range; i++) {
		for (size_t j = 0; j < this->hidden_range; j++)
			y_predict[i] += _W_y[i][j] * h[k + 1][j];
		y_predict[i] = sigm(y_predict[i]); // приводим предсказание к необходимой форме
	}
	return y_predict;
}

double LSTM::calculate_error(double* y_predict, double* y_real)
{
	double error = 0;
	for(size_t i = 0; i < this->output_range; i++)
		error += 0.5 * (y_real[i] - y_predict[i]) * (y_real[i] - y_predict[i]);

	return error;
}

void LSTM::calculate_gates(double*& forgate_gate, double*& x, double*& input_gate, double*& output_gate, double*& state_gate, const size_t k)
{
	// расчёт модели LSTM
	for (int i = 0; i < this->hidden_range; i++) {
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
		forgate_gate[i] = sigm(forgate_gate[i]);
		input_gate[i] = sigm(input_gate[i]);
		output_gate[i] = sigm(output_gate[i]);
		state_gate[i] = tanh(state_gate[i]);
	}
}

void LSTM::fit(DataVector& train_data, double target_error, long long batch_size) {
	// создаём и конфигурируем скейлер
	Scaler scaler;
	auto max_element = train_data.get_max_element();
	auto min_element = train_data.get_min_element();
	auto average_value = train_data.get_average_value();
 	scaler.configure(max_element.value + fabs(average_value) / 3, min_element.value - fabs(average_value) / 3);

	// скейлим входные данные
	scaler.scale(train_data);

	// для сбора мусора
	double* garbage_catcher = nullptr;

	// подготовка к обучению
	size_t work_size = train_data.get_size() - (this->input_range + this->output_range - 1); // допустимая размерность для работы
	double** x = new double* [work_size]; // матрица входных цепочек для каждой итерации
	double** y_real = new double* [work_size]; // вектор реальных значений каждой итерации
	C = new double* [work_size + 1]; // + 1 для хранеия значений C[t + 1] (следующих значений)
	h = new double* [work_size + 1];
	for (size_t i = 0; i <= work_size; i++) { // орагнизуем начальную долгосрочную и краткосрочную память
		C[i] = new double[this->hidden_range] {0};
		h[i] = new double[this->hidden_range] {0};
	}

	// настраиваем групповое обучение
	if (batch_size <= 0 || batch_size > work_size)
		batch_size = work_size;
	long long batch_interator; // чтобы знать, сколько пачек подали

	// выделяем память под промежуточные веса
	select_memory_for_temp_weight();

#ifdef ErrorInfo
	// создаём файл ошибок
	std::ofstream error_file;
	error_file.open("Error.csv");
#endif // ErrorInfo

	// обучение сети
	for (size_t i = 0; i < this->epochs; i++) {
		std::cout << "epoch = " << i + 1 << "/" << this->epochs << " ";
		std::clock_t start, finish;
		start = clock();

		// сохряняем текущие значения весов
		copy_weight();

		// обнуляем среднюю квадратичную ошибку эпохи предсказаний и итератор подачи пачек
		predict_error = 0;
		batch_interator = 0;

		// формируем входные цепочки и их соотвесттвующие реальные значения, с которыми будет проводиться сравнение предсказаний
		for (int j = 0; j < work_size; j++) {
			x[j] = create_input_vector(train_data, this->input_range, j);
			y_real[j] = create_input_vector(train_data, this->output_range, j + this->input_range);
		}
		
		while (batch_size * batch_interator < work_size) {
			// прогонозируем, чтобы получить значения памяти на каждой итерации 
			for (size_t j = batch_size * batch_interator; j < batch_size * (batch_interator + 1) && j < work_size; j++) {
				garbage_catcher = forecast(x[j], j); // этот цикл нужен, чтобы расчитать каждые вектора C и h для каждой итерации 
				delete[] garbage_catcher;
			}

			// обозначим, что подали одну пачку 
			batch_interator++;

			// защитим от выхода за границы массива
			long long j;
			if (batch_size * batch_interator > work_size)
				j = work_size - 1;
			else
				j = batch_size * batch_interator - 1;

			// обучаем сеть
			for (j; j >= batch_size * (batch_interator - 1); j--) {
				train(x[j], y_real[j], j); // метод обратного распространения во времени		
				delete[] x[j]; // удаление для дальнейшего обновления
				delete[] y_real[j];
			}

			// обнуляем ошибки по будущему
			this->clear_futur();
		}

#ifdef ErrorInfo
		// записываем результаты ошибки в файл
		error_file << i << ";" << predict_error << "\n";
#endif // ErrorInfo

		finish = clock();
		std::cout << "| time = " << (double)(finish - start) / CLOCKS_PER_SEC << " sec "
			<< "| E = " << predict_error << "\n";

		if (target_error > predict_error)
			break;
	}

#ifdef ErrorInfo
	// закрываем поток работы с файлом ошибок
	error_file.close();
#endif // ErrorInfo

	// сохраняем память сети после последнего её расчёта при обучении
	copy_vector(last_C_from_train, C[work_size], this->hidden_range);
	copy_vector(last_h_from_train, h[work_size], this->hidden_range);

	// очищаем память
	free_temp_weigth();
	for (int i = 0; i <= work_size; i++) {
		delete[] C[i];
		delete[] h[i];
	}
	delete[] C;
	delete[] h;
	delete[] x;
	delete[] y_real;

	// восстанавливаем данные
	scaler.unscale(train_data);
}

double* LSTM::forecast(double* x, size_t k) {
	// создаём вентили и расчитываем модель сети
	double* forgate_gate = new double[this->hidden_range] {0};
	double* input_gate = new double[this->hidden_range] {0};
	double* output_gate = new double[this->hidden_range] {0};
	double* state_gate = new double[this->hidden_range] {0};
	for (int i = 0; i < this->hidden_range; i++) {  
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
		forgate_gate[i] = sigm(forgate_gate[i]); 
		input_gate[i] = sigm(input_gate[i]); 
		output_gate[i] = sigm(output_gate[i]);
		state_gate[i] = tanh(state_gate[i]);
		// расчитываем новую долгосрочную память
		C[k + 1][i] = forgate_gate[i] * C[k][i] + input_gate[i] * state_gate[i];
		// расчитываем новую краткосрочную память
		h[k + 1][i] = output_gate[i] * tanh(C[k + 1][i]);
	}

	// очищаем память, выделенную на вентили сети
	delete[] forgate_gate;
	delete[] input_gate;
	delete[] state_gate;
	delete[] output_gate;

	// возвращаем предстказание сети
	return calculate_output(k);
}

void LSTM::predict(DataVector& test_data) {
	// создаём и конфигурируем скейлер
	Scaler scaler;
	auto max_element = test_data.get_max_element();
	auto min_element = test_data.get_min_element();
	auto average_value = test_data.get_average_value();
	scaler.configure(max_element.value + fabs(average_value) / 3, min_element.value - fabs(average_value) / 3);

	// скейлим входные данные
	scaler.scale(test_data);

	// предполагается, что расстояние между точками по оси Х равное, поэтому вычисляем его
	size_t dist = test_data.count_distance();
	
	// создаём временный вектор с увелеченный на output_range памятью
	DataVector temp(1);
	temp = test_data;
	temp.resize(test_data.get_size() + this->output_range);

	// создаём долгосрочную и краткосрочную память
	C = new double* [2];
	h = new double* [2];
	C[0] = nullptr;
	h[0] = nullptr;
	copy_vector(C[0], last_C_from_train, this->hidden_range);
	copy_vector(h[0], last_h_from_train, this->hidden_range);
	C[1] = new double[this->hidden_range] {0};
	h[1] = new double[this->hidden_range] {0};

	select_memory_for_temp_weight();
	copy_weight();

	// выполняем предсказания
	double* x = nullptr;
	double* y_predict = nullptr;
	bool out_falg = true;
	for (size_t i = 0; i < test_data.get_size() && out_falg; i = i + this->output_range) {
		if (i > test_data.get_size() - this->input_range) {
			i = test_data.get_size() - this->input_range;
			out_falg = false;
		}
		x = create_input_vector(test_data, this->input_range, i);
		y_predict = forecast(x); 
		for (int j = 0; j < this->output_range; j++) {
			temp[i + this->input_range + j].cid = test_data[0].cid;
			temp[i + this->input_range + j].time = test_data[0].time + dist * (i + this->input_range + j);
			size_t temp_time = temp[i + this->input_range + j].time;
			temp[i + this->input_range + j].value = y_predict[j];
		}
		copy_vector(C[0], C[1], this->hidden_range);
		copy_vector(h[0], h[1], this->hidden_range);
		delete[] x;
		delete[] y_predict;
	}

	// сохраняем результаты предсказаний как реальные данные
	test_data = temp;

	// очищаем долгосрочную и краткосрочную память
	delete[] C[1];
	delete[] C[0];
	delete[] C;

	delete[] h[1];
	delete[] h[0];
	delete[] h;

	free_temp_weigth();

	// востанавливаем значиня из промежутка [0, 1] в нормальный промежуток
	scaler.unscale(test_data);
}

LSTM::LSTM(double learning_rate, unint16 epochs, unint16 input_range, unint16 hidden_range, unint16 output_range) : RecurrentNeuron(learning_rate, epochs, input_range) {
	this->hidden_range = hidden_range;
	this->output_range = output_range;

	// параметрые, представляющие память сети
	de_dF_future = new double[this->hidden_range] {0};
	de_dO_future = new double[this->hidden_range] {0};
	de_dG_future = new double[this->hidden_range] {0};
	de_dI_future = new double[this->hidden_range] {0};
	de_dC_future = new double[this->hidden_range] {0};
	forgate_future = new double[this->hidden_range] {0};

	// веса 
	int deep = 10; 
	create_random_matrix(W_f, hidden_range, input_range, deep);
	create_random_matrix(W_i, hidden_range, input_range, deep);
	create_random_matrix(W_g, hidden_range, input_range, deep);
	create_random_matrix(W_o, hidden_range, input_range, deep);

	create_random_matrix(U_f, hidden_range, hidden_range, deep);
	create_random_matrix(U_i, hidden_range, hidden_range, deep);
	create_random_matrix(U_g, hidden_range, hidden_range, deep);
	create_random_matrix(U_o, hidden_range, hidden_range, deep);

	create_random_matrix(W_y, output_range, hidden_range, deep);
}

LSTM::~LSTM() {
	// параметрые, представляющие память сети
	delete[] de_dF_future;
	delete[] de_dO_future;
	delete[] de_dI_future;
	delete[] de_dG_future;
	delete[] de_dC_future;
	delete[] forgate_future;

	// веса 
	delete_matrix(W_f, hidden_range);
	delete_matrix(W_i, hidden_range);
	delete_matrix(W_g, hidden_range);
	delete_matrix(W_o, hidden_range);

	delete_matrix(U_f, hidden_range);
	delete_matrix(U_i, hidden_range);
	delete_matrix(U_g, hidden_range);
	delete_matrix(U_o, hidden_range);

	delete_matrix(W_y, output_range);
}


void load_model(LSTM& lstm, string file_name) {
	std::ifstream work_file(file_name, std::ios::binary | std::ios::in);
	if (!work_file.is_open()) {
		system("cls");
		std::cerr << "\n!Load model error: model file is not open!\n";
		exit(0);
	}

	// ranges
	unint16 temp_range;
	work_file.read((char*)&temp_range, sizeof(lstm.input_range));
	if (temp_range != lstm.input_range) {
		system("cls");
		std::cerr << "\n!Load model error: invalid input range\n";
		exit(0);
	}
	work_file.read((char*)&temp_range, sizeof(lstm.hidden_range));
	if (temp_range != lstm.hidden_range) {
		system("cls");
		std::cerr << "\n!Load model error: invalid hidden range\n";
		exit(0);
	}
	work_file.read((char*)&temp_range, sizeof(lstm.output_range));
	if (temp_range != lstm.output_range) {
		system("cls");
		std::cerr << "\n!Load model error: invalid output range\n";
		exit(0);
	}

	// параметры
	work_file.read((char*)&lstm.learning_rate, sizeof(double));
	work_file.read((char*)&lstm.predict_error, sizeof(double));
	work_file.read((char*)&lstm.epochs, sizeof(unint16));

	lstm.last_h_from_train = new double[lstm.hidden_range];
	lstm.last_C_from_train = new double[lstm.hidden_range];
	
	for (size_t i = 0; i < lstm.hidden_range; i++) {
		for (size_t j = 0; j < lstm.hidden_range; j++) {
			work_file.read((char*)&lstm.U_f[i][j], sizeof(lstm.U_f[i][j]));
			work_file.read((char*)&lstm.U_i[i][j], sizeof(lstm.U_i[i][j]));
			work_file.read((char*)&lstm.U_o[i][j], sizeof(lstm.U_o[i][j]));
			work_file.read((char*)&lstm.U_g[i][j], sizeof(lstm.U_g[i][j]));
		}
		for (size_t j = 0; j < lstm.input_range; j++) {
			work_file.read((char*)&lstm.W_f[i][j], sizeof(lstm.W_f[i][j]));
			work_file.read((char*)&lstm.W_i[i][j], sizeof(lstm.W_i[i][j]));
			work_file.read((char*)&lstm.W_o[i][j], sizeof(lstm.W_o[i][j]));
			work_file.read((char*)&lstm.W_g[i][j], sizeof(lstm.W_g[i][j]));
		}
		for (size_t j = 0; j < lstm.output_range; j++)
			work_file.read((char*)&lstm.W_y[j][i], sizeof(lstm.W_y[j][i]));
		work_file.read((char*)&lstm.last_h_from_train[i], sizeof(lstm.last_h_from_train[i]));
		work_file.read((char*)&lstm.last_C_from_train[i], sizeof(lstm.last_C_from_train[i]));
	}
	work_file.close();
}

void dump_model(LSTM& lstm, string file_name) {
	if (lstm.last_h_from_train == nullptr || lstm.last_C_from_train == nullptr) {
		system("cls");
		std::cerr << "\n!Dump model error: the model has no memory!\n";
		exit(0);
	}

	std::ofstream result_file(file_name, std::ios::binary | std::ios::out);

	// ranges
	result_file.write((char*)&lstm.input_range, sizeof(lstm.input_range));
	result_file.write((char*)&lstm.hidden_range, sizeof(lstm.hidden_range));
	result_file.write((char*)&lstm.output_range, sizeof(lstm.output_range));

	// параметры
	result_file.write((char*)&lstm.learning_rate, sizeof(lstm.learning_rate));
	result_file.write((char*)&lstm.predict_error, sizeof(lstm.predict_error));
	result_file.write((char*)&lstm.epochs, sizeof(lstm.epochs));

	// веса и память 
	for (size_t i = 0; i < lstm.hidden_range; i++) {
		for (size_t j = 0; j < lstm.hidden_range; j++) {
			result_file.write((char*)&lstm.U_f[i][j], sizeof(lstm.U_f[i][j]));
			result_file.write((char*)&lstm.U_i[i][j], sizeof(lstm.U_i[i][j]));
			result_file.write((char*)&lstm.U_o[i][j], sizeof(lstm.U_o[i][j]));
			result_file.write((char*)&lstm.U_g[i][j], sizeof(lstm.U_g[i][j]));
		}
		for (size_t j = 0; j < lstm.input_range; j++) {
			result_file.write((char*)&lstm.W_f[i][j], sizeof(lstm.W_f[i][j]));
			result_file.write((char*)&lstm.W_i[i][j], sizeof(lstm.W_i[i][j]));
			result_file.write((char*)&lstm.W_o[i][j], sizeof(lstm.W_o[i][j]));
			result_file.write((char*)&lstm.W_g[i][j], sizeof(lstm.W_g[i][j]));
		}
		for (size_t j = 0; j < lstm.output_range; j++)
			result_file.write((char*)&lstm.W_y[j][i], sizeof(lstm.W_y[j][i]));
		result_file.write((char*)&lstm.last_h_from_train[i], sizeof(lstm.last_h_from_train[i]));
		result_file.write((char*)&lstm.last_C_from_train[i], sizeof(lstm.last_C_from_train[i]));
	}
	result_file.close();
}
